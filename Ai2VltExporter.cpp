#include <ostream>

#include "IllustratorSDK.h"
#include "Ai2VltExporter.h"
#include "Ai2VltSuites.h"
#include "Utility.h"
#include "ezxml/ezxml.h"

#undef strcpy

static void addHookData(ezxml_t node, AIArtHandle art)
{
	AIDictionaryRef dictionary = nullptr;
	AIDictionaryRef paramsDictionary = nullptr;
	AIDictionaryIterator paramsIt = nullptr;
	try
	{
		CHECK(sAIArt->GetDictionary(art, &dictionary));
		AIDictKey key = sAIDictionary->Key("vltHook");
		const char * hook = nullptr;
		CHECK_EXCEPT(sAIDictionary->GetStringEntry(dictionary, key, &hook), kNoSuchKey);

		if (hook)
			ezxml_set_attr_d(node, "hook", hook);

		AIDictKey paramsKey = sAIDictionary->Key("vltParams");
		CHECK_EXCEPT(sAIDictionary->GetDictEntry(dictionary, paramsKey, &paramsDictionary), kNoSuchKey);
		if (paramsDictionary)
		{
			char paramsBuf[128];
			char * paramsBufS = paramsBuf;
			CHECK(sAIDictionary->Begin(paramsDictionary, &paramsIt));
			while (!sAIDictionaryIterator->AtEnd(paramsIt))
			{
				AIDictKey paramKey = sAIDictionaryIterator->GetKey(paramsIt);
				const char * value = nullptr;
				CHECK(sAIDictionary->GetStringEntry(paramsDictionary, paramKey, &value));

				if (paramsBufS != paramsBuf)
					*(paramsBufS++) = ' ';
				strcpy(paramsBufS, sAIDictionary->GetKeyString(paramKey));
				paramsBufS += strlen(sAIDictionary->GetKeyString(paramKey));
				*(paramsBufS++) = ':';
				strcpy(paramsBufS, value);
				paramsBufS += strlen(value);

				sAIDictionaryIterator->Next(paramsIt);
			}
			sAIDictionaryIterator->Release(paramsIt);
			sAIDictionary->Release(paramsDictionary);
			ezxml_set_attr_d(node, "params", paramsBuf);
		}

		sAIDictionary->Release(dictionary);
	}
	catch (Ai2Vlt::Error & ex)
	{
		if (dictionary)
			sAIDictionary->Release(dictionary);
		if (paramsIt)
			sAIDictionaryIterator->Release(paramsIt);
		if (paramsDictionary)
			sAIDictionary->Release(paramsDictionary);
		ex.report();
		throw;
	}
}

static void gatherArt(AIArtSet set, AIArtHandle art, AIRealRect & setBounds)
{
	try
	{
		short type = 0;
		CHECK(sAIArt->GetArtType(art, &type));
		switch (type)
		{
		case kGroupArt:
		{
			AIArtHandle child;
			CHECK(sAIArt->GetArtFirstChild(art, &child));
			while (child)
			{
				gatherArt(set, child, setBounds);
				CHECK(sAIArt->GetArtSibling(child, &child));
			}
		}
			break;
		case kUnknownArt:
			break;
		default:
		{
			CHECK(sAIArtSet->AddArtToArtSet(set, art));
			AIRealRect bounds;
			CHECK(sAIArt->GetArtBounds(art, &bounds));
			sAIRealMath->AIRealRectUnion(&bounds, &setBounds, &setBounds);
		}
			break;
		}
	}
	catch (Ai2Vlt::Error & ex)
	{
		ex.report();
		throw;
	}
}

static void exportImage(AIArtHandle art, const ai::FilePath & name)
{
	AIArtSet set = nullptr;
	AIArtHandle raster = nullptr;
	AIDataFilter * filter = nullptr;
	AIRealRect bounds;
	try
	{
		CHECK(sAIArtSet->NewArtSet(&set));
		CHECK(sAIArt->GetArtBounds(art, &bounds));
		gatherArt(set, art, bounds);

		AIRasterizeSettings settings{ kRasterizeARGB, 300, 0, kRasterizeOptionsNone, AIColorConvertOptions{}, false };
		CHECK(sAIRasterize->Rasterize(set, &settings, &bounds, kPlaceAboveAll, nullptr, &raster, nullptr));

		CHECK(sAIDataFilter->NewFileDataFilter(name, "write", 0, 0, &filter));
		CHECK(sAIDataFilter->LinkDataFilter(NULL, filter));
		AIImageOptPNGParams params{ false, 255, 254, 300, true, bounds.right - bounds.left, bounds.top - bounds.bottom };
		CHECK(sAIImageOpt->AsPNG(raster, filter, params));

		// TODO(rgriege): delete me
		const std::string & n = name.GetFullPath(true).as_Platform();
		printf(n.c_str());

		sAIArt->DisposeArt(raster);
		sAIArtSet->DisposeArtSet(&set);
		sAIDataFilter->UnlinkDataFilter(filter, nullptr);
	}
	catch (Ai2Vlt::Error & ex)
	{
		if (raster)
			sAIArt->DisposeArt(raster);
		if (set)
			sAIArtSet->DisposeArtSet(&set);
		if (filter)
			sAIDataFilter->UnlinkDataFilter(filter, nullptr);

		ex.report();
		throw;
	}
}

static void exportRectangle(ezxml_t node, AIArtHandle artHandle, size_t n, bool insideSymbol, AIRealPoint origin)
{
	AIDictionaryRef dictionary = nullptr;
	try
	{
		AIPathStyle style;
		CHECK(sAIPathStyle->GetPathStyle(artHandle, &style));
		if (!style.clip)
		{
			CHECK(sAIArt->GetDictionary(artHandle, &dictionary));
			AIDictKey key = sAIDictionary->Key("vltHook");
			const char * hook = nullptr;
			CHECK_EXCEPT(sAIDictionary->GetStringEntry(dictionary, key, &hook), kNoSuchKey);
			const char * childName = hook ? "btn" : "rect";
			sAIDictionary->Release(dictionary);

			ezxml_t child = ezxml_add_child(node, childName, n);
			AIPathSegment segment;
			AIRealRect artBounds;
			CHECK(sAIArt->GetArtBounds(artHandle, &artBounds));
			char buf[32];
			if (!insideSymbol)
			{
				sprintf(buf, "%.0f", artBounds.left - origin.h);
				ezxml_set_attr_d(child, "x", buf);
				sprintf(buf, "%.0f", artBounds.bottom - origin.v);
				ezxml_set_attr_d(child, "y", buf);
			}
			else
			{
				AIRealRect maxBounds;
				CHECK(sAIDocument->GetDocumentMaxArtboardBounds(&maxBounds));
				sprintf(buf, "%.0f", artBounds.left - maxBounds.left - origin.h);
				ezxml_set_attr_d(child, "x", buf);
				sprintf(buf, "%.0f", artBounds.bottom - maxBounds.top - origin.v);
				ezxml_set_attr_d(child, "y", buf);
			}

			CHECK(sAIPath->GetPathSegments(artHandle, 3, 1, &segment));
			sprintf(buf, "%.0f", artBounds.right - artBounds.left);
			ezxml_set_attr_d(child, "width", buf);
			sprintf(buf, "%.0f", artBounds.top - artBounds.bottom);
			ezxml_set_attr_d(child, "height", buf);

			if (style.fillPaint && style.fill.color.kind == kThreeColor)
			{
				sprintf(buf, "#%.2x%.2x%.2x",
					(unsigned)(255 * style.fill.color.c.rgb.red),
					(unsigned)(255 * style.fill.color.c.rgb.green),
					(unsigned)(255 * style.fill.color.c.rgb.blue));
				ezxml_set_attr_d(child, "fill", buf);
			}

			if (style.strokePaint && style.stroke.color.kind == kThreeColor)
			{
				sprintf(buf, "#%.2x%.2x%.2x",
					(unsigned)(255 * style.stroke.color.c.rgb.red),
					(unsigned)(255 * style.stroke.color.c.rgb.green),
					(unsigned)(255 * style.stroke.color.c.rgb.blue));
				ezxml_set_attr_d(child, "stroke", buf);

				if ((unsigned)(style.stroke.width) > 1)
				{
					sprintf(buf, "%u", (unsigned)style.stroke.width);
					ezxml_set_attr_d(child, "stroke-width", buf);
				}
			}

			addHookData(child, artHandle);
		}
	}
	catch (Ai2Vlt::Error & ex)
	{
		if (dictionary)
			sAIDictionary->Release(dictionary);
		ex.report();
		throw;
	}
}

static void exportLine(ezxml_t node, AIArtHandle artHandle, size_t n, bool insideSymbol, AIRealPoint origin)
{
	try
	{
		AIPathStyle style;
		CHECK(sAIPathStyle->GetPathStyle(artHandle, &style));
		if (!style.clip)
		{
			ezxml_t child = ezxml_add_child(node, "line", n);
			char buf[32];
			AIRealRect maxBounds = { 0, 0, 0, 0 };
			if (insideSymbol)
				CHECK(sAIDocument->GetDocumentMaxArtboardBounds(&maxBounds));

			ai::int16 segmentCount;
			CHECK(sAIPath->GetPathSegmentCount(artHandle, &segmentCount));
			for (ai::int16 i = 0; i < segmentCount; ++i)
			{
				AIPathSegment segment;
				CHECK(sAIPath->GetPathSegments(artHandle, i, 1, &segment));
				char label_buf[4];
				sprintf(label_buf, "x%d", i);
				sprintf(buf, "%.0f", segment.p.h - maxBounds.left - origin.v);
				ezxml_set_attr_d(child, label_buf, buf);
				sprintf(label_buf, "y%d", i);
				sprintf(buf, "%.0f", segment.p.v - maxBounds.top - origin.h);
				ezxml_set_attr_d(child, label_buf, buf);
			}

			if (style.strokePaint && style.stroke.color.kind == kThreeColor)
			{
				sprintf(buf, "#%.2x%.2x%.2x",
					(unsigned)(255 * style.stroke.color.c.rgb.red),
					(unsigned)(255 * style.stroke.color.c.rgb.green),
					(unsigned)(255 * style.stroke.color.c.rgb.blue));
				ezxml_set_attr_d(child, "stroke", buf);

				if ((unsigned)(style.stroke.width) > 1)
				{
					sprintf(buf, "%u", (unsigned)style.stroke.width);
					ezxml_set_attr_d(child, "stroke-width", buf);
				}
			}
		}
	}
	catch (Ai2Vlt::Error & ex)
	{
		ex.report();
		throw;
	}
}

static void exportText(ezxml_t node, AIArtHandle artHandle, size_t n, bool insideSymbol, AIRealPoint origin)
{
	try
	{
		char buf[16];
		ezxml_t text = ezxml_add_child(node, "text", n);

		TextRangeRef textRange;
		CHECK(sAITextFrame->GetATETextRange(artHandle, &textRange));
		ATE::ITextRange iTextRange(textRange);

		ATE::ICharInspector charInspector = iTextRange.GetCharInspector();
		const double fontSize = charInspector.GetFontSize().GetFirst();

		AIRealRect bounds;
		CHECK(sATETextUtil->GetBoundsFromTextRange(textRange, &bounds));

		bool justificationAssigned;
		ATE::ParagraphJustification justification = iTextRange.GetUniqueParaFeatures().GetJustification(&justificationAssigned);
		if (!justificationAssigned)
			justification = ATE::kLeftJustify;
		switch (justification)
		{
		case ATE::kRightJustify:
			ezxml_set_attr(text, "font-align", "right");
			sprintf(buf, "%.0f", bounds.right - origin.v);
			ezxml_set_attr_d(text, "x", buf);
			break;
		case ATE::kCenterJustify:
			ezxml_set_attr(text, "font-align", "center");
			sprintf(buf, "%.0f", bounds.left + (bounds.right - bounds.left) / 2.f - origin.v);
			ezxml_set_attr_d(text, "x", buf);
			break;
		case ATE::kLeftJustify:
			sprintf(buf, "%.0f", bounds.left - origin.v);
			ezxml_set_attr_d(text, "x", buf);
		default:
			break;
		}
		sprintf(buf, "%.0f", bounds.bottom - fontSize - origin.h);
		ezxml_set_attr_d(text, "y", buf);

		ASInt32 strLength = iTextRange.GetSize();
		if (strLength > 0)
		{
			std::vector<char> vc(strLength);
			ASInt32 conLength = iTextRange.GetContents(vc.data(), strLength);
			if (conLength == strLength)
			{
				std::string contents;
				contents.assign(vc.begin(), vc.begin() + strLength);
				ezxml_set_txt_d(text, contents.c_str());
			}
		}

		sprintf(buf, "%.0f", fontSize);
		ezxml_set_attr_d(text, "font-size", buf);

		ATE::IFont font = charInspector.GetFont().GetFirst();
		AIFontKey fontKey;
		CHECK(sAIFont->FontKeyFromFont(font.GetRef(), &fontKey));
		char fontName[128];
		CHECK(sAIFont->GetSystemFontName(fontKey, fontName, 128));
		const size_t fontNameSz = strlen(fontName);
		fontName[fontNameSz] = '-';
		CHECK(sAIFont->GetFontStyleName(fontKey, fontName + fontNameSz + 1, 128 - fontNameSz - 1));
		ezxml_set_attr_d(text, "font-family", fontName);

		AIColor fontColor;
		CHECK(sATEPaint->GetAIColor(charInspector.GetFillColor().GetFirst().GetRef(), &fontColor));
		if (fontColor.kind == kThreeColor)
		{
			sprintf(buf, "#%.2x%.2x%.2x",
				(unsigned)(255 * fontColor.c.rgb.red),
				(unsigned)(255 * fontColor.c.rgb.green),
				(unsigned)(255 * fontColor.c.rgb.blue));
			ezxml_set_attr_d(text, "fill", buf);
		}

		addHookData(text, artHandle);
	}
	catch (Ai2Vlt::Error & ex)
	{
		ex.report();
		throw;
	}
}

static void exportArt(ezxml_t node, AIArtHandle artHandle, size_t n, bool insideSymbol, AIRealPoint origin, std::vector<AIArtHandle> & images)
{
	try
	{
		short type = 0;
		CHECK(sAIArt->GetArtType(artHandle, &type));
		switch (type)
		{
		case kUnknownArt:
			ezxml_add_child(node, "unknown", n);
			break;
		case kGroupArt:
		{
			AIBoolean isDefaultName;
			ai::UnicodeString name;
			CHECK(sAIArt->GetArtName(artHandle, name, &isDefaultName));
			if (!isDefaultName && name.find(ai::UnicodeString("img:")) == 0)
			{
				images.emplace_back(artHandle);
			}
			else
			{
				AIArtHandle childHandle;
				CHECK(sAIArt->GetArtFirstChild(artHandle, &childHandle));
				size_t childIdx = 0;
				while (childHandle)
				{
					exportArt(node, childHandle, childIdx++, insideSymbol, origin, images);
					CHECK(sAIArt->GetArtSibling(childHandle, &childHandle));
				}
			}
			break;
		}
		case kPathArt:
		{
			if (Ai2Vlt::isShape(artHandle, "Rectangle"))
				exportRectangle(node, artHandle, n, insideSymbol, origin);
			else if (Ai2Vlt::isShape(artHandle, "Line"))
				exportLine(node, artHandle, n, insideSymbol, origin);
			else
			{
				AIBoolean isDefaultName;
				ai::UnicodeString name;
				CHECK(sAIArt->GetArtName(artHandle, name, &isDefaultName));
				ezxml_t path = ezxml_add_child(node, "path", n);
				ezxml_set_attr_d(path, "name", name.as_Platform().c_str());
				ezxml_set_attr(path, "default", isDefaultName ? "true" : "false");
			}
			break;
		}
		case kCompoundPathArt:
			ezxml_add_child(node, "compound_path", n);
			break;
		case kPlacedArt:
			ezxml_add_child(node, "placed", n);
			break;
		case kRasterArt:
			ezxml_add_child(node, "raster", n);
			break;
		case kPluginArt:
			ezxml_add_child(node, "plugin", n);
			break;
		case kMeshArt:
			ezxml_add_child(node, "mesh", n);
			break;
		case kTextFrameArt:
			exportText(node, artHandle, n, insideSymbol, origin);
			break;
		case kSymbolArt:
		{
			ezxml_t symbol = ezxml_add_child(node, "use", n);
			AIPatternHandle symbolHandle;
			CHECK(sAISymbol->GetSymbolPatternOfSymbolArt(artHandle, &symbolHandle));
			if (symbolHandle)
			{
				ai::UnicodeString symbolName;
				CHECK(sAISymbol->GetSymbolPatternName(symbolHandle, symbolName));
				char buf[64];
				sprintf(buf, "#%s", symbolName.as_Platform().c_str());
				ezxml_set_attr_d(symbol, "xlink:href", buf);


				AIRealRect artBounds;
				CHECK(sAIArt->GetArtBounds(artHandle, &artBounds));
				sprintf(buf, "%.0f", artBounds.left - origin.h);
				ezxml_set_attr_d(symbol, "x", buf);
				sprintf(buf, "%.0f", artBounds.bottom - origin.v);
				ezxml_set_attr_d(symbol, "y", buf);

				addHookData(symbol, artHandle);
			}
			break;
		}
		default:
			ezxml_add_child(node, "unsupported", n);
			break;
		}
	}
	catch (Ai2Vlt::Error & ex)
	{
		ex.report();
		throw;
	}
}

static void exportDocument(ezxml_t doc, const char * filename)
{
	AILayerHandle tempLayer = nullptr;
	try
	{
		std::vector<AIArtHandle> images;

		ai::ArtboardList artboards;
		CHECK(sAIArtboard->GetArtboardList(artboards));
		ai::ArtboardProperties artboardProperties;
		CHECK(sAIArtboard->GetArtboardProperties(artboards, 0, artboardProperties));
		AIRealRect bounds;
		CHECK(artboardProperties.GetPosition(bounds));
		{
			char buf[32];
			sprintf(buf, "%d %d %d %d",
				(int)bounds.bottom,
				(int)bounds.left,
				(int)bounds.right,
				(int)bounds.top);
			ezxml_set_attr_d(doc, "viewBox", buf);
		}

		ai::int32 symbolCount;
		CHECK(sAISymbol->CountSymbolPatterns(&symbolCount, false));
		for (ai::int32 i = 0; i < symbolCount; ++i)
		{
			AIPatternHandle symbolHandle;
			CHECK(sAISymbol->GetNthSymbolPattern(i, &symbolHandle, false));
			if (sAISymbol->ArtUsesSymbolPattern(NULL, kSymbolSearchEverywhere, symbolHandle, NULL, NULL, NULL, NULL))
			{
				// TODO(rgriege): remove me
				ai::UnicodeString symbolName;
				CHECK(sAISymbol->GetSymbolPatternName(symbolHandle, symbolName));
				ezxml_t symbol = ezxml_add_child(doc, "symbol", i);
				ezxml_set_attr_d(symbol, "id", symbolName.as_Platform().c_str());

				AIArtHandle artHandle;
				CHECK(sAISymbol->GetSymbolPatternArt(symbolHandle, &artHandle));

				AIRealRect artBounds, maxBounds;
				CHECK(sAIArt->GetArtBounds(artHandle, &artBounds));
				CHECK(sAIDocument->GetDocumentMaxArtboardBounds(&maxBounds));

				exportArt(symbol, artHandle, 0, true, AIRealPoint{ artBounds.left - maxBounds.left, artBounds.bottom - maxBounds.top }, images);
			}
		}

		ai::int32 layerCount = 0;
		sAILayer->CountLayers(&layerCount);
		for (ai::int32 i = 0; i < layerCount; ++i)
		{
			AILayerHandle layerHandle;
			sAILayer->GetNthLayer(layerCount - i - 1, &layerHandle);
			AIBoolean visible;
			sAILayer->GetLayerVisible(layerHandle, &visible);
			if (visible)
			{
				// TODO(rgriege): remove me
				ai::UnicodeString layerName;
				sAILayer->GetLayerTitle(layerHandle, layerName);
				ezxml_t layer = ezxml_add_child(doc, "g", symbolCount + i);
				ezxml_set_attr_d(layer, "id", layerName.as_Platform().c_str());

				AIArtHandle artHandle;
				sAIArt->GetFirstArtOfLayer(layerHandle, &artHandle);
				exportArt(layer, artHandle, 0, false, AIRealPoint{ 0, 0 }, images);
			}
		}

		const ai::UnicodeString dir = ai::FilePath{ ai::UnicodeString(filename) }.GetDirectory();
		sAILayer->InsertLayer(nullptr, kPlaceAboveAll, &tempLayer);
		for (AIArtHandle art : images)
		{
			AIBoolean isDefaultName;
			ai::UnicodeString name;
			sAIArt->GetArtName(art, name, &isDefaultName);

			ai::FilePath filePath{ dir };
			filePath.AddComponent(name.substr(4));
			filePath.AddExtension(".png");
			exportImage(art, filePath);
		}
		sAILayer->DeleteLayer(tempLayer);
	}
	catch (Ai2Vlt::Error & ex)
	{
		if (tempLayer)
			sAILayer->DeleteLayer(tempLayer);
		ex.report();
		throw;
	}
}

void Ai2Vlt::exportSvg(const char * filename)
{
	std::ofstream file;
	file.open(filename, std::ios::out);
	if (file.is_open())
	{
		ezxml_t doc = ezxml_new("svg");
		bool success = false;
		try
		{
			exportDocument(doc, filename);
			success = true;
		}
		catch (Ai2Vlt::Error & ex)
		{
			ex.report();
		}
		if (success)
		{
			char * xml = ezxml_toxml(doc);
			file << xml;
			free(xml);
		}
		ezxml_free(doc);
		file.close();
	}
}