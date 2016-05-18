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

extern "C" AIUnicodeStringSuite*			sAIUnicodeString;
extern "C" SPBlocksSuite*					sSPBlocks;
extern "C" AIArtboardSuite*					sAIArtboard;
extern "C" AIMenuSuite*						sAIMenu;
extern "C" AIDataFilterSuite*				sAIDataFilter;
extern "C" AIDocumentListSuite*				sAIDocumentList;
extern "C" AIDocumentSuite*					sAIDocument;
extern "C" AIArtSuite*						sAIArt;
extern "C" AIDictionarySuite*				sAIDictionary;
extern "C" AIArtSetSuite*					sAIArtSet;
extern "C" AISymbolSuite*					sAISymbol;
extern "C" AIRasterizeSuite*				sAIRasterize;
extern "C" AILayerSuite*					sAILayer;
extern "C" AIPathStyleSuite*				sAIPathStyle;
extern "C" AIMatchingArtSuite*				sAIMatchingArt;
extern "C" AITextFrameSuite*				sAITextFrame;
extern "C" AIColorConversionSuite*			sAIColorConversion;
extern "C" AIFontSuite*						sAIFont;
extern "C" AIPathSuite*						sAIPath;
extern "C" AIATETextUtilSuite*				sATETextUtil;
extern "C" AIDictionaryIteratorSuite*		sAIDictionaryIterator;
extern "C" AIFileFormatSuite*				sAIFileFormat;
extern "C" AIMdMemorySuite*					sAIMdMemory;
extern "C" AIATEPaintSuite*					sATEPaint;
extern "C" AIImageOptSuite*					sAIImageOpt;
extern "C" AIRealMathSuite*					sAIRealMath;

#endif
