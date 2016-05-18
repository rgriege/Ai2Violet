#include "IllustratorSDK.h"
#include "Ai2VltSuites.h"
#include "ATETextSuitesImportHelper.h"

extern "C"
{
	AIUnicodeStringSuite*				sAIUnicodeString = NULL;
	SPBlocksSuite*						sSPBlocks = NULL;
	AIArtboardSuite*					sAIArtboard = NULL;
	AIMenuSuite*						sAIMenu = NULL;
	AIDataFilterSuite*					sAIDataFilter = NULL;
	AIDocumentListSuite*				sAIDocumentList = NULL;
	AIDocumentSuite*					sAIDocument = NULL;
	AIArtSuite*							sAIArt = NULL;
	AIDictionarySuite*					sAIDictionary = NULL;
	AIArtSetSuite*						sAIArtSet = NULL;
	AISymbolSuite*						sAISymbol = NULL;
	AIRasterizeSuite*					sAIRasterize = NULL;
	AILayerSuite*						sAILayer = NULL;
	AIPathStyleSuite*					sAIPathStyle = NULL;
	AIMatchingArtSuite*					sAIMatchingArt = NULL;
	AITextFrameSuite*					sAITextFrame = NULL;
	AIColorConversionSuite*				sAIColorConversion = NULL;
	AIFontSuite*						sAIFont = NULL;
	AIPathSuite*						sAIPath = NULL;
	AIATETextUtilSuite*					sATETextUtil = NULL;
	AIDictionaryIteratorSuite*			sAIDictionaryIterator = NULL;
	AIFileFormatSuite*					sAIFileFormat = NULL;
	AIMdMemorySuite*					sAIMdMemory = NULL;
	AIATEPaintSuite*					sATEPaint = NULL;
	AIImageOptSuite*					sAIImageOpt = NULL;
	AIRealMathSuite*					sAIRealMath = NULL;
	EXTERN_TEXT_SUITES
};

ImportSuite gImportSuites[] = 
{
	kAIUnicodeStringSuite, kAIUnicodeStringSuiteVersion, &sAIUnicodeString,
	kAIArtboardSuite, kAIArtboardSuiteVersion, &sAIArtboard,
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,
	kAIMenuSuite, kAIMenuSuiteVersion, &sAIMenu,
	kAIDataFilterSuite, kAIDataFilterSuiteVersion, &sAIDataFilter,
	kAIDocumentListSuite, kAIDocumentListSuiteVersion, &sAIDocumentList,
	kAIDocumentSuite, kAIDocumentSuiteVersion, &sAIDocument,
	kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
	kAIDictionarySuite, kAIDictionarySuiteVersion, &sAIDictionary,
	kAIArtSetSuite, kAIArtSetSuiteVersion, &sAIArtSet,
	kAISymbolSuite, kAISymbolSuiteVersion, &sAISymbol,
	kAIRasterizeSuite, kAIRasterizeSuiteVersion, &sAIRasterize,
	kAILayerSuite, kAILayerSuiteVersion, &sAILayer,
	kAIPathStyleSuite, kAIPathStyleSuiteVersion, &sAIPathStyle,
	kAIMatchingArtSuite, kAIMatchingArtSuiteVersion, &sAIMatchingArt,
	kAITextFrameSuite, kAITextFrameSuiteVersion, &sAITextFrame,
	kAIColorConversionSuite, kAIColorConversionSuiteVersion, &sAIColorConversion,
	kAIFontSuite, kAIFontSuiteVersion, &sAIFont,
	kAIPathSuite, kAIPathSuiteVersion, &sAIPath,
	kAIATETextUtilSuite, kAIATETextUtilSuiteVersion, &sATETextUtil,
	kAIDictionaryIteratorSuite, kAIDictionaryIteratorSuiteVersion, &sAIDictionaryIterator,
	kAIFileFormatSuite, kAIFileFormatSuiteVersion, &sAIFileFormat,
	kAIMdMemorySuite, kAIMdMemorySuiteVersion, &sAIMdMemory,
	kAIATEPaintSuite, kAIATEPaintSuiteVersion, &sATEPaint,
	kAIImageOptSuite, kAIImageOptSuiteVersion, &sAIImageOpt,
	kAIRealMathSuite, kAIRealMathSuiteVersion, &sAIRealMath,

	IMPORT_TEXT_SUITES
	nil, 0, nil
};

