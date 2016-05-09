#include "IllustratorSDK.h"
#include "Ai2VltSuites.h"

extern "C"
{
	AIUnicodeStringSuite*	sAIUnicodeString = NULL;
	SPBlocksSuite*			sSPBlocks = NULL;
	AIFileFormatSuite*		sAIFileFormat = NULL;
	AIDocumentSuite*		sAIDocument = NULL;
	AITextFrameSuite*		sAITextFrame = NULL;
	AIArtSuite*				sAIArt = NULL;
	AIPathSuite*			sAIPath = NULL;
	AIMatchingArtSuite*		sAIMatchingArt = NULL;
	AIMdMemorySuite*		sAIMdMemory = NULL;
	EXTERN_TEXT_SUITES

	AIPathStyleSuite *sAIPathStyle = NULL;
	AIHardSoftSuite *sAIHardSoft = NULL;
	AIRealMathSuite *sAIRealMath = NULL;
	AIGradientSuite *sAIGradient = NULL;
	AIMaskSuite *sAIMask = NULL;
	AIPluginGroupSuite *sAIPluginGroup = NULL;
	AICustomColorSuite *sAICustomColor = NULL;
	AIColorConversionSuite *sAIColorConversion = NULL;
	AIBlendStyleSuite *sAIBlendStyle = NULL;
	AILayerSuite *sAILayer = NULL;
	AIATEPaintSuite *sATEPaint = NULL;
	AIFontSuite *sAIFont = NULL;
	AIATETextUtilSuite *sATETextUtil = NULL;
	AIDataFilterSuite *sAIDataFilter = NULL;
	AISymbolSuite *sAISymbol = NULL;
	AIPatternSuite *sAIPattern = NULL;
	AIPlacedSuite *sAIPlaced = NULL;
	AIRasterSuite *sAIRaster = NULL;
	AIImageOptSuite *sAIImageOpt = NULL;
	AIArtStyleSuite *sAIArtStyle = NULL;
	AIArtStyleParserSuite *sAIArtStyleParser = NULL;
	AILiveEffectSuite *sAILiveEffect = NULL;
	AIDictionarySuite *sAIDictionary = NULL;
	AIDictionaryIteratorSuite *sAIDictionaryIterator = NULL;
	AIEntrySuite *sAIEntry = NULL;
	AIRealBezierSuite *sAIRealBezier = NULL;
};

ImportSuite gImportSuites[] = 
{
	kAIUnicodeStringSuite, kAIUnicodeStringSuiteVersion, &sAIUnicodeString,
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,
	kAIFileFormatSuite, kAIFileFormatVersion, &sAIFileFormat,
	kAIDocumentSuite, kAIDocumentVersion, &sAIDocument,
	kAITextFrameSuite, kAITextFrameVersion, &sAITextFrame,
	kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
	kAIPathSuite, kAIPathVersion, &sAIPath,
	kAIMatchingArtSuite, kAIMatchingArtVersion, &sAIMatchingArt,
	kAIMdMemorySuite, kAIMdMemorySuiteVersion, &sAIMdMemory,

	kAIPathStyleSuite, kAIPathStyleVersion, &sAIPathStyle,
	kAIHardSoftSuite, kAIHardSoftVersion, &sAIHardSoft,
	kAIRealMathSuite, kAIRealMathVersion, &sAIRealMath,
	kAIGradientSuite, kAIGradientVersion, &sAIGradient,
	kAIMaskSuite, kAIMaskVersion, &sAIMask,
	kAIPluginGroupSuite, kAIPluginGroupVersion, &sAIPluginGroup,
	kAICustomColorSuite, kAICustomColorVersion, &sAICustomColor,
	kAIColorConversionSuite, kAIColorConversionVersion, &sAIColorConversion,
	kAIBlendStyleSuite, kAIBlendStyleVersion, &sAIBlendStyle,
	kAILayerSuite, kAILayerVersion, &sAILayer,
	kAIATEPaintSuite, kAIATEPaintSuiteVersion, &sATEPaint,
	kAIFontSuite, kAIFontSuiteVersion, &sAIFont,
	kAIATETextUtilSuite, kAIATETextUtilSuiteVersion, &sATETextUtil,
	kAIDataFilterSuite, kAIDataFilterSuiteVersion, &sAIDataFilter,
	kAISymbolSuite, kAISymbolSuiteVersion, &sAISymbol,
	kAIPatternSuite, kAIPatternSuiteVersion, &sAIPattern,
	kAIPlacedSuite, kAIPlacedSuiteVersion, &sAIPlaced,
	kAIRasterSuite, kAIRasterSuiteVersion, &sAIRaster,
	kAIArtStyleSuite, kAIArtStyleSuiteVersion, &sAIArtStyle,
    kAIArtStyleParserSuite, kAIArtStyleParserSuiteVersion, &sAIArtStyleParser,
	kAILiveEffectSuite, kAILiveEffectSuiteVersion, &sAILiveEffect,
	kAIDictionarySuite, kAIDictionarySuiteVersion, &sAIDictionary,
	kAIDictionaryIteratorSuite, kAIDictionaryIteratorSuiteVersion, &sAIDictionaryIterator,
	kAIEntrySuite, kAIEntrySuiteVersion, &sAIEntry,
	kAIImageOptSuite, kAIImageOptSuiteVersion, &sAIImageOpt,
	kAIRealBezierSuite, kAIRealBezierSuiteVersion, &sAIRealBezier,

	IMPORT_TEXT_SUITES
	nil, 0, nil
};

