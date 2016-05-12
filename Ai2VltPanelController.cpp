#include "IllustratorSDK.h"

#include <assert.h>

#include "Ai2VltPanelController.h"
#include "AppContext.hpp"
#include "SDKErrors.h"
#include "Plugin.hpp"
#include "Utility.h"
#include "json/json.h"

#define AI2VLT_UI_EXTENSION				"com.bf.Ai2VltUi"
#define ILST_APPID						"ILST"

// Events we listen out for
#define EVENT_TYPE_PANEL_READY			"com.adobe.csxs.events.Ai2Vlt.PanelReady"
#define EVENT_TYPE_LOAD_HOOKS			"com.adobe.csxs.events.Ai2Vlt.LoadHooks"
#define EVENT_TYPE_SELECT_HOOK			"com.adobe.csxs.events.Ai2Vlt.SelectHook"

// Events we dispatch
#define EVENT_UPDATE_SELECTION			"com.adobe.csxs.events.Ai2Vlt.UpdateSelection"
#define EVENT_UPDATE_HOOK_FILE			"com.adobe.csxs.events.Ai2Vlt.UpdateHookFile"

static std::string getRelativePath(ai::FilePath base, const ai::FilePath & file)
{
	ai::UnicodeString fileStr = file.GetFullPath();
	std::string relativePath("");
	while (fileStr.find(base.GetDirectory()) != 0)
	{
		static const char s_back[4] = { '.', '.', ai::FilePath::GetDelimiter(), '\0' };
		base = base.GetParent();
		relativePath.append(s_back);
	}

	relativePath.append(fileStr.substr(base.GetDirectory().length()).as_Platform());

	return relativePath;
}

static std::string getFullPath(const ai::FilePath & base, const ai::FilePath & file)
{
	static const char s_back[4] = { '.', '.', ai::FilePath::GetDelimiter(), '\0' };

	ai::FilePath fullPath{ base.GetDirectory() };
	std::string filePath = file.GetFullPath().as_Platform();
	while (filePath.find(s_back) == 0)
	{
		fullPath = fullPath.GetParent();
		filePath.erase(0, 3);
	}

	return fullPath.GetFullPath().as_Platform() + filePath;
}


static void PanelReadyFunc(const csxs::event::Event* const eventParam, void* const context)
{
	Ai2VltPanelController *ai2vltPanelController = (Ai2VltPanelController *)context;
	if (ai2vltPanelController == nullptr || eventParam == nullptr)
		return;

	AppContext ctx(ai2vltPanelController->m_pluginRef);
	ai2vltPanelController->ArtSelectionChanged();
	ai2vltPanelController->SendHooksToPanel();
}

static void LoadHooksFunc(const csxs::event::Event* const eventParam, void* const context)
{
	Ai2VltPanelController *ai2vltPanelController = (Ai2VltPanelController *)context;
	if (ai2vltPanelController == nullptr || eventParam == nullptr)
		return;

	AppContext ctx(ai2vltPanelController->m_pluginRef);
	ai2vltPanelController->LoadHooks();
}

static void SelectHookFunc(const csxs::event::Event* const eventParam, void* const context)
{
	Ai2VltPanelController *ai2vltPanelController = (Ai2VltPanelController *)context;
	if (ai2vltPanelController == nullptr || eventParam == nullptr)
		return;

	AppContext ctx(ai2vltPanelController->m_pluginRef);
	ai2vltPanelController->SelectHook(eventParam->data);
}

Ai2VltPanelController::Ai2VltPanelController(SPPluginRef pluginRef) :
	HtmlUIController(AI2VLT_UI_EXTENSION),
	m_pluginRef(pluginRef),
	m_docDictionary(nullptr)
{
}

Ai2VltPanelController::~Ai2VltPanelController()
{
	if (m_docDictionary)
		sAIDictionary->Release(m_docDictionary);
}

csxs::event::EventErrorCode Ai2VltPanelController::RegisterCSXSEventListeners()
{
	csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
	result = htmlPPLib.AddEventListener(EVENT_TYPE_PANEL_READY, PanelReadyFunc, this);
	if (result != csxs::event::kEventErrorCode_Success) return result;
	result = htmlPPLib.AddEventListener(EVENT_TYPE_LOAD_HOOKS, LoadHooksFunc, this);
	if (result != csxs::event::kEventErrorCode_Success) return result;
	result = htmlPPLib.AddEventListener(EVENT_TYPE_SELECT_HOOK, SelectHookFunc, this);
	return result;
}

csxs::event::EventErrorCode Ai2VltPanelController::RemoveEventListeners()
{
	csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
	result = htmlPPLib.RemoveEventListener(EVENT_TYPE_PANEL_READY, PanelReadyFunc, this);
	if (result != csxs::event::kEventErrorCode_Success) return result;
	result = htmlPPLib.RemoveEventListener(EVENT_TYPE_LOAD_HOOKS, LoadHooksFunc, this);
	if (result != csxs::event::kEventErrorCode_Success) return result;
	result = htmlPPLib.RemoveEventListener(EVENT_TYPE_SELECT_HOOK, SelectHookFunc, this);
	return result;
}

void Ai2VltPanelController::CurrentDocumentChanged()
{
	AIDictionaryRef dictionary;
	sAIDocument->GetDictionary(&dictionary);
	if (m_docDictionary != dictionary)
	{
		if (m_docDictionary)
			sAIDictionary->Release(m_docDictionary);
		m_docDictionary = dictionary;
		if (m_docDictionary)
			sAIDictionary->AddRef(m_docDictionary);

		SendHooksToPanel();
	}
}

void Ai2VltPanelController::SendHooksToPanel()
{
	if (m_docDictionary)
	{
		AIDictKey key = sAIDictionary->Key("violetHookFile");
		const char * hookFileEntry = nullptr;
		sAIDictionary->GetStringEntry(m_docDictionary, key, &hookFileEntry);
		if (hookFileEntry)
		{
			ai::FilePath hookFile = ai::FilePath(ai::UnicodeString(hookFileEntry));
			std::string hookFileName = hookFile.GetFileName().as_Platform();

			ai::FilePath aiFile;
			sAIDocument->GetDocumentFileSpecification(aiFile);
			std::string const hookFilePath = getFullPath(aiFile, hookFile);
			std::ifstream stream;
			stream.open(hookFilePath.c_str(), std::ios::in);
			Json::Reader reader;
			Json::Value root;
			if (!reader.parse(stream, root, false))
			{
				root.clear();
				root["hookFileName"] = "<Hook File>";
			}
			else
				root["hookFileName"] = hookFileName;

			Json::FastWriter writer;
			std::string payload = writer.write(root);
			std::remove(payload.begin(), payload.end(), '\\');
			csxs::event::Event event = { EVENT_UPDATE_HOOK_FILE,
				csxs::event::kEventScope_Application,
				ILST_APPID,
				nullptr,
				payload.c_str() };

			htmlPPLib.DispatchEvent(&event);
		}
	}
}

void Ai2VltPanelController::LoadHooks()
{
	assert(m_docDictionary);
	ASErr err = kNoErr;

	ai::FilePath aiFile;
	err = sAIDocument->GetDocumentFileSpecification(aiFile);
	if (err) return;

	AIFileDialogFilters filters;
	filters.AddFilter(ai::UnicodeString("JSON"), ai::UnicodeString("*.json"));
	ai::FilePath hookFile = aiFile;
	err = sAIUser->GetFileDialog(ai::UnicodeString("Choose hook file"), &filters, hookFile);
	if (err) return;
	const std::string path = getRelativePath(aiFile, hookFile);

	AIDictKey key = sAIDictionary->Key("violetHookFile");
	err = sAIDictionary->SetStringEntry(m_docDictionary, key, path.c_str());
	if (err) return;

	SendHooksToPanel();
}

void Ai2VltPanelController::ArtSelectionChanged()
{
	ai::int32 artCount = 0;
	AIArtHandle **artStore = nullptr;
	AIMatchingArtSpec spec;
	spec.type = kAnyArt;
	spec.whichAttr = kArtFullySelected;
	spec.attr = kArtFullySelected;
	sAIMatchingArt->GetMatchingArt(&spec, 1, &artStore, &artCount);
	if (artCount == 1)
	{
		AIArtHandle art = (*artStore)[0];
		SetArt(art);
		sAIMdMemory->MdMemoryDisposeHandle((void**)artStore);
	}
	else
		SetArt(nullptr);
}

enum VioletArtType
{
	VAT_None = 0,
	VAT_Btn,
	VAT_Text,
	VAT_SymbolRef
};

static const char * g_artTypeStrings[] = {
	"",
	"Button",
	"Text",
	"Symbol"
};

void Ai2VltPanelController::SetArt(AIArtHandle art)
{
	Json::Value root{ Json::objectValue };
	VioletArtType type = VAT_None;
	if (art != nullptr)
	{
		short aiType;
		sAIArt->GetArtType(art, &aiType);

		switch (aiType)
		{
		case kPathArt:
			if (isShape(art, "Rectangle"))
				type = VAT_Btn;
			break;
		case kTextFrameArt:
			type = VAT_Text;
			break;
		case kSymbolArt:
			type = VAT_SymbolRef;
			break;
		default:
			break;
		}

		AIDictionaryRef dictionary;
		sAIArt->GetDictionary(art, &dictionary);
		AIDictionaryIterator it;
		sAIDictionary->Begin(dictionary, &it);
		while (!sAIDictionaryIterator->AtEnd(it))
		{
			AIDictKey key = sAIDictionaryIterator->GetKey(it);
			AIEntryType entryType;
			sAIDictionary->GetEntryType(dictionary, key, &entryType);
			const char * keyName = sAIDictionary->GetKeyString(key);
			switch (entryType)
			{
			case StringType:
				if (strcmp(keyName, "vltHook") == 0)
				{
					const char * value = nullptr;
					sAIDictionary->GetStringEntry(dictionary, key, &value);
					root["hook"] = value;
				}
				break;
			case DictType:
				if (strcmp(keyName, "vltParams") == 0)
				{
					AIDictionaryRef paramsDictionary;
					sAIDictionary->GetDictEntry(dictionary, key, &paramsDictionary);
					AIDictionaryIterator paramsIt;
					sAIDictionary->Begin(paramsDictionary, &paramsIt);
					while (!sAIDictionaryIterator->AtEnd(paramsIt))
					{
						AIDictKey paramKey = sAIDictionaryIterator->GetKey(paramsIt);
						const char * value = nullptr;
						sAIDictionary->GetStringEntry(paramsDictionary, paramKey, &value);
						root["params"][sAIDictionary->GetKeyString(paramKey)] = value;
						sAIDictionaryIterator->Next(paramsIt);
					}
					sAIDictionaryIterator->Release(paramsIt);
					sAIDictionary->Release(paramsDictionary);
				}
				break;
			}
			sAIDictionaryIterator->Next(it);
		}
		sAIDictionaryIterator->Release(it);
		sAIDictionary->Release(dictionary);
	}

	root["artType"] = g_artTypeStrings[type];
	
	Json::FastWriter writer;
	std::string payload = writer.write(root);
	std::remove(payload.begin(), payload.end(), '\\');

	csxs::event::Event event = { EVENT_UPDATE_SELECTION,
		csxs::event::kEventScope_Application,
		ILST_APPID,
		nullptr,
		payload.c_str() };

	htmlPPLib.DispatchEvent(&event);
}

void Ai2VltPanelController::SelectHook(const char * hook)
{
	ai::int32 artCount = 0;
	AIArtHandle **artStore = nullptr;
	AIMatchingArtSpec spec;
	spec.type = kAnyArt;
	spec.whichAttr = kArtFullySelected;
	spec.attr = kArtFullySelected;
	sAIMatchingArt->GetMatchingArt(&spec, 1, &artStore, &artCount);
	assert(artCount == 1);
	AIArtHandle art = (*artStore)[0];

	AIDictionaryRef dictionary;
	sAIArt->GetDictionary(art, &dictionary);
	AIDictKey key = sAIDictionary->Key("vltHook");
	sAIDictionary->SetStringEntry(dictionary, key, hook);

	sAIDictionary->Release(dictionary);
	sAIMdMemory->MdMemoryDisposeHandle((void**)artStore);
}