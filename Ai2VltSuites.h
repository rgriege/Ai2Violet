#ifndef __AI2VLTSUITES_H__
#define __AI2VLTSUITES_H__

#include "IllustratorSDK.h"
#include "Suites.hpp"

#include "AIArt.h"
#include "AIArtboard.h"
#include "AIArtSet.h"
#include "AIATEPaint.h"
#include "AIATETextUtil.h"
#include "AIColorConversion.h"
#include "AIDataFilter.h"
#include "AIDictionary.h"
#include "AIDocumentList.h"
#include "AIFont.h"
#include "AIImageOptimization.h"
#include "AILayer.h"
#include "AIMatchingArt.h"
#include "AIPath.h"
#include "AIPathStyle.h"
#include "AIRasterize.h"
#include "AIRealMath.h"
#include "AISymbol.h"
#include "AITextFrame.h"

extern "C" AIArtSuite*						sAIArt;
extern "C" AIArtboardSuite*					sAIArtboard;
extern "C" AIArtSetSuite*					sAIArtSet;
extern "C" AIColorConversionSuite*			sAIColorConversion;
extern "C" AIDataFilterSuite*				sAIDataFilter;
extern "C" AIDictionarySuite*				sAIDictionary;
extern "C" AIDictionaryIteratorSuite*		sAIDictionaryIterator;
extern "C" AIDocumentSuite*					sAIDocument;
extern "C" AIDocumentListSuite*				sAIDocumentList;
extern "C" AIFileFormatSuite*				sAIFileFormat;
extern "C" AIFontSuite*						sAIFont;
extern "C" AIImageOptSuite*					sAIImageOpt;
extern "C" AILayerSuite*					sAILayer;
extern "C" AIMatchingArtSuite*				sAIMatchingArt;
extern "C" AIMdMemorySuite*					sAIMdMemory;
extern "C" AIMenuSuite*						sAIMenu;
extern "C" AIPathSuite*						sAIPath;
extern "C" AIPathStyleSuite*				sAIPathStyle;
extern "C" AIRasterizeSuite*				sAIRasterize;
extern "C" AIRealMathSuite*					sAIRealMath;
extern "C" AISymbolSuite*					sAISymbol;
extern "C" AITextFrameSuite*				sAITextFrame;
extern "C" AIUnicodeStringSuite*			sAIUnicodeString;
extern "C" AIATEPaintSuite*					sATEPaint;
extern "C" AIATETextUtilSuite*				sATETextUtil;
extern "C" SPBlocksSuite*					sSPBlocks;

#endif
