#include "IllustratorSDK.h"
#include "Ai2VltSuites.h"
#include "ATETextSuitesImportHelper.h"

extern "C"
{
	AIArtSuite*							sAIArt = NULL;
	AIArtboardSuite*					sAIArtboard = NULL;
	AIArtSetSuite*						sAIArtSet = NULL;
	AIColorConversionSuite*				sAIColorConversion = NULL;
	AIDataFilterSuite*					sAIDataFilter = NULL;
	AIDictionarySuite*					sAIDictionary = NULL;
	AIDictionaryIteratorSuite*			sAIDictionaryIterator = NULL;
	AIDocumentSuite*					sAIDocument = NULL;
	AIDocumentListSuite*				sAIDocumentList = NULL;
	AIFileFormatSuite*					sAIFileFormat = NULL;
	AIFontSuite*						sAIFont = NULL;
	AIImageOptSuite*					sAIImageOpt = NULL;
	AILayerSuite*						sAILayer = NULL;
	AIMatchingArtSuite*					sAIMatchingArt = NULL;
	AIMdMemorySuite*					sAIMdMemory = NULL;
	AIMenuSuite*						sAIMenu = NULL;
	AIPathSuite*						sAIPath = NULL;
	AIPathStyleSuite*					sAIPathStyle = NULL;
	AIRasterizeSuite*					sAIRasterize = NULL;
	AIRealMathSuite*					sAIRealMath = NULL;
	AISymbolSuite*						sAISymbol = NULL;
	AITextFrameSuite*					sAITextFrame = NULL;
	AIUnicodeStringSuite*				sAIUnicodeString = NULL;
	AIATEPaintSuite*					sATEPaint = NULL;
	AIATETextUtilSuite*					sATETextUtil = NULL;
	SPBlocksSuite*						sSPBlocks = NULL;
	EXTERN_TEXT_SUITES
};

ImportSuite gImportSuites[] = 
{
	kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
	kAIArtboardSuite, kAIArtboardSuiteVersion, &sAIArtboard,
	kAIArtSetSuite, kAIArtSetSuiteVersion, &sAIArtSet,
	kAIColorConversionSuite, kAIColorConversionSuiteVersion, &sAIColorConversion,
	kAIDataFilterSuite, kAIDataFilterSuiteVersion, &sAIDataFilter,
	kAIDictionarySuite, kAIDictionarySuiteVersion, &sAIDictionary,
	kAIDictionaryIteratorSuite, kAIDictionaryIteratorSuiteVersion, &sAIDictionaryIterator,
	kAIDocumentSuite, kAIDocumentSuiteVersion, &sAIDocument,
	kAIDocumentListSuite, kAIDocumentListSuiteVersion, &sAIDocumentList,
	kAIFileFormatSuite, kAIFileFormatSuiteVersion, &sAIFileFormat,
	kAIFontSuite, kAIFontSuiteVersion, &sAIFont,
	kAIImageOptSuite, kAIImageOptSuiteVersion, &sAIImageOpt,
	kAILayerSuite, kAILayerSuiteVersion, &sAILayer,
	kAIMatchingArtSuite, kAIMatchingArtSuiteVersion, &sAIMatchingArt,
	kAIMdMemorySuite, kAIMdMemorySuiteVersion, &sAIMdMemory,
	kAIMenuSuite, kAIMenuSuiteVersion, &sAIMenu,
	kAIPathSuite, kAIPathSuiteVersion, &sAIPath,
	kAIPathStyleSuite, kAIPathStyleSuiteVersion, &sAIPathStyle,
	kAIRasterizeSuite, kAIRasterizeSuiteVersion, &sAIRasterize,
	kAIRealMathSuite, kAIRealMathSuiteVersion, &sAIRealMath,
	kAISymbolSuite, kAISymbolSuiteVersion, &sAISymbol,
	kAITextFrameSuite, kAITextFrameSuiteVersion, &sAITextFrame,
	kAIUnicodeStringSuite, kAIUnicodeStringSuiteVersion, &sAIUnicodeString,
	kAIATEPaintSuite, kAIATEPaintSuiteVersion, &sATEPaint,
	kAIATETextUtilSuite, kAIATETextUtilSuiteVersion, &sATETextUtil,
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,

	IMPORT_TEXT_SUITES
	nil, 0, nil
};

