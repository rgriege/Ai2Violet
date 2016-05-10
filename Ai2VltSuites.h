#ifndef __AI2VLTSUITES_H__
#define __AI2VLTSUITES_H__

#include "IllustratorSDK.h"
#include "Suites.hpp"
#include "ATETextSuitesImportHelper.h"

#include "AIArtboard.h"
#include "AIColorConversion.h"
#include "AIATEPaint.h"
#include "AIATETextUtil.h"
#include "AIImageOptimization.h"
#include "AISymbol.h"
#include "AIArtStyleParser.h"
#include "AIPattern.h"
#include "AIPathStyle.h"
#include "AIGradient.h"

extern	"C"	AIUnicodeStringSuite*	sAIUnicodeString;
extern  "C" SPBlocksSuite*			sSPBlocks;
extern	"C" AIFileFormatSuite*		sAIFileFormat;
extern	"C" AIDocumentSuite*		sAIDocument;
extern	"C" AITextFrameSuite*		sAITextFrame;
extern	"C" AIArtSuite*				sAIArt;
extern	"C" AIPathSuite*			sAIPath;
extern	"C" AIMatchingArtSuite*		sAIMatchingArt;
extern	"C" AIMdMemorySuite*		sAIMdMemory;

extern "C" AIArtboardSuite *sAIArtboard;
extern "C" AIATEPaintSuite *sATEPaint;
extern "C" AIFontSuite *sAIFont;
extern "C" AIATETextUtilSuite *sATETextUtil;
extern "C" AIDataFilterSuite *sAIDataFilter;
extern "C" AISymbolSuite *sAISymbol;
extern "C" AIPatternSuite *sAIPattern;
extern "C" AIPlacedSuite *sAIPlaced;
extern "C" AIRasterSuite *sAIRaster;
extern "C" AIImageOptSuite *sAIImageOpt;
extern "C" AIArtStyleSuite *sAIArtStyle;
extern "C" AIArtStyleParserSuite *sAIArtStyleParser;
extern "C" AILiveEffectSuite *sAILiveEffect;
extern "C" AIDictionarySuite *sAIDictionary;
extern "C" AIDictionaryIteratorSuite *sAIDictionaryIterator;
extern "C" AIEntrySuite *sAIEntry;
extern "C" AIPathStyleSuite *sAIPathStyle;
extern "C" AIHardSoftSuite *sAIHardSoft;
extern "C" AIRealMathSuite *sAIRealMath;
extern "C" AIGradientSuite *sAIGradient;
extern "C" AIMaskSuite *sAIMask;
extern "C" AIPluginGroupSuite *sAIPluginGroup;
extern "C" AICustomColorSuite *sAICustomColor;
extern "C" AIColorConversionSuite *sAIColorConversion;
extern "C" AIBlendStyleSuite *sAIBlendStyle;
extern "C" AILayerSuite *sAILayer;
extern "C" AIRealBezierSuite *sAIRealBezier;

#endif
