#include <ostream>

#include "IllustratorSDK.h"
#include "Ai2VltExporter.h"
#include "Ai2VltSuites.h"
#include "Utility.h"
#include "ezxml/ezxml.h"

#undef strcpy

static void addHookData(ezxml_t node, AIArtHandle art)
{
	AIDictionaryRef dictionary;
	sAIArt->GetDictionary(art, &dictionary);
	AIDictKey key = sAIDictionary->Key("vltHook");
	const char * hook = nullptr;
	sAIDictionary->GetStringEntry(dictionary, key, &hook);

	if (hook)
		ezxml_set_attr_d(node, "hook", hook);

	AIDictKey paramsKey = sAIDictionary->Key("vltParams");
	AIDictionaryRef paramsDictionary = nullptr;
	sAIDictionary->GetDictEntry(dictionary, paramsKey, &paramsDictionary);
	if (paramsDictionary)
	{
		char paramsBuf[128];
		char * paramsBufS = paramsBuf;
		AIDictionaryIterator paramsIt;
		sAIDictionary->Begin(paramsDictionary, &paramsIt);
		while (!sAIDictionaryIterator->AtEnd(paramsIt))
		{
			AIDictKey paramKey = sAIDictionaryIterator->GetKey(paramsIt);
			const char * value = nullptr;
			sAIDictionary->GetStringEntry(paramsDictionary, paramKey, &value);

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

static void exportArt(ezxml_t node, AIArtHandle artHandle, size_t n, bool insideSymbol)
{
	short type = 0;
	sAIArt->GetArtType(artHandle, &type);
	switch (type)
	{
	case kUnknownArt:
		ezxml_add_child(node, "unknown", n);
		break;
	case kGroupArt:
	{
		AIArtHandle childHandle;
		sAIArt->GetArtFirstChild(artHandle, &childHandle);
		size_t childIdx = 0;
		while (childHandle)
		{
			exportArt(node, childHandle, childIdx++, insideSymbol);
			sAIArt->GetArtSibling(childHandle, &childHandle);
		}
		break;
	}
	case kPathArt:
	{
		if (isShape(artHandle, "Rectangle"))
		{
			AIPathStyle style;
			sAIPathStyle->GetPathStyle(artHandle, &style);
			if (!style.clip)
			{
				AIDictionaryRef dictionary;
				sAIArt->GetDictionary(artHandle, &dictionary);
				AIDictKey key = sAIDictionary->Key("vltHook");
				const char * hook = nullptr;
				sAIDictionary->GetStringEntry(dictionary, key, &hook);
				const char * childName = hook ? "btn" : "rect";
				sAIDictionary->Release(dictionary);

				ezxml_t child = ezxml_add_child(node, childName, n);
				AIPathSegment segment;
				AIRealRect artBounds;
				sAIArt->GetArtBounds(artHandle, &artBounds);
				char buf[32];
				if (!insideSymbol)
				{
					sprintf(buf, "%.0f", artBounds.left);
					ezxml_set_attr_d(child, "x", buf);
					sprintf(buf, "%.0f", artBounds.bottom);
					ezxml_set_attr_d(child, "y", buf);
				}
				else
				{
					AIRealRect maxBounds;
					sAIDocument->GetDocumentMaxArtboardBounds(&maxBounds);
					sprintf(buf, "%.0f", artBounds.left - maxBounds.left);
					ezxml_set_attr_d(child, "x", buf);
					sprintf(buf, "%.0f", artBounds.bottom - maxBounds.top);
					ezxml_set_attr_d(child, "y", buf);
				}

				sAIPath->GetPathSegments(artHandle, 3, 1, &segment);
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

					if ((unsigned)(style.stroke.miterLimit) != 1)
					{
						sprintf(buf, "%u", (unsigned)style.stroke.miterLimit);
						ezxml_set_attr_d(child, "stroke-miterlimit", buf);
					}
				}

				addHookData(child, artHandle);
			}
		}
		else
		{
			AIBoolean isDefaultName;
			ai::UnicodeString name;
			sAIArt->GetArtName(artHandle, name, &isDefaultName);
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
	{
		char buf[16];
		ezxml_t text = ezxml_add_child(node, "text", n);

		AIRealRect artBounds;
		sAIArt->GetArtBounds(artHandle, &artBounds);
		sprintf(buf, "%.0f", artBounds.left);
		ezxml_set_attr_d(text, "x", buf);
		sprintf(buf, "%.0f", artBounds.bottom);
		ezxml_set_attr_d(text, "y", buf);

		TextRangeRef textRange;
		sAITextFrame->GetATETextRange(artHandle, &textRange);
		ATE::ITextRange iTextRange(textRange);
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

		ATE::ICharInspector charInspector = iTextRange.GetCharInspector();
		const double fontSize = charInspector.GetFontSize().GetFirst();
		sprintf(buf, "%.0f", fontSize);
		ezxml_set_attr_d(text, "font-size", buf);

		ATE::IFont font = charInspector.GetFont().GetFirst();
		AIFontKey fontKey;
		sAIFont->FontKeyFromFont(font.GetRef(), &fontKey);
		char fontName[128];
		sAIFont->GetSystemFontName(fontKey, fontName, 128);
		const size_t fontNameSz = strlen(fontName);
		fontName[fontNameSz] = '-';
		sAIFont->GetFontStyleName(fontKey, fontName + fontNameSz + 1, 128 - fontNameSz - 1);
		ezxml_set_attr_d(text, "font-family", fontName);

		AIColor fontColor;
		sATEPaint->GetAIColor(charInspector.GetFillColor().GetFirst().GetRef(), &fontColor);
		if (fontColor.kind == kThreeColor)
		{
			sprintf(buf, "#%.2x%.2x%.2x",
				(unsigned)(255 * fontColor.c.rgb.red),
				(unsigned)(255 * fontColor.c.rgb.green),
				(unsigned)(255 * fontColor.c.rgb.blue));
			ezxml_set_attr_d(text, "fill", buf);
		}

		addHookData(text, artHandle);
		break;
	}
	case kSymbolArt:
	{
		ezxml_t symbol = ezxml_add_child(node, "use", n);
		AIPatternHandle symbolHandle;
		sAISymbol->GetSymbolPatternOfSymbolArt(artHandle, &symbolHandle);
		if (symbolHandle)
		{
			ai::UnicodeString symbolName;
			sAISymbol->GetSymbolPatternName(symbolHandle, symbolName);
			char buf[64];
			sprintf(buf, "#%s", symbolName.as_Platform().c_str());
			ezxml_set_attr_d(symbol, "xlink:href", buf);


			AIRealRect artBounds;
			sAIArt->GetArtBounds(artHandle, &artBounds);
			sprintf(buf, "%.0f", artBounds.left);
			ezxml_set_attr_d(symbol, "x", buf);
			sprintf(buf, "%.0f", artBounds.bottom);
			ezxml_set_attr_d(symbol, "y", buf);
		}
		break;
	}
	default:
		ezxml_add_child(node, "unsupported", n);
		break;
	}
}

static void exportDocument(ezxml_t doc)
{
	ai::ArtboardList artboards;
	sAIArtboard->GetArtboardList(artboards);
	ai::ArtboardProperties artboardProperties;
	sAIArtboard->GetArtboardProperties(artboards, 0, artboardProperties);
	AIRealRect bounds;
	artboardProperties.GetPosition(bounds);
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
	sAISymbol->CountSymbolPatterns(&symbolCount, false);
	for (ai::int32 i = 0; i < symbolCount; ++i)
	{
		AIPatternHandle symbolHandle;
		sAISymbol->GetNthSymbolPattern(i, &symbolHandle, false);
		if (sAISymbol->ArtUsesSymbolPattern(NULL, kSymbolSearchEverywhere, symbolHandle, NULL, NULL, NULL, NULL))
		{
			// TODO(rgriege): remove me
			ai::UnicodeString symbolName;
			sAISymbol->GetSymbolPatternName(symbolHandle, symbolName);
			ezxml_t symbol = ezxml_add_child(doc, "symbol", i);
			ezxml_set_attr_d(symbol, "id", symbolName.as_Platform().c_str());

			AIArtHandle artHandle;
			sAISymbol->GetSymbolPatternArt(symbolHandle, &artHandle);

			exportArt(symbol, artHandle, 0, true);
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
			exportArt(layer, artHandle, 0, false);
		}
	}
}

void Ai2Vlt::exportSvg(const char * filename)
{
	std::ofstream file;
	file.open(filename, std::ios::out);
	if (file.is_open())
	{
		ezxml_t doc = ezxml_new("svg");
		exportDocument(doc);
		char * xml = ezxml_toxml(doc);
		file << xml;
		free(xml);
		ezxml_free(doc);
		file.close();
	}
}
