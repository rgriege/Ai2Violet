#include "IllustratorSDK.h"

#include <assert.h>

#include "Ai2VltPanelController.h"
#include "AppContext.hpp"
#include "SDKErrors.h"
#include "Plugin.hpp"
#include "Utility.h"
extern "C" {
#include "ezxml/ezxml.h"
}

#define AI2VLT_UI_EXTENSION				"com.bf.Ai2VltUi"
#define ILST_APPID						"ILST"

// Events we listen out for
#define EVENT_TYPE_PANEL_READY			"com.adobe.csxs.events.Ai2Vlt.PanelReady"
#define EVENT_TYPE_LOAD_HOOKS			"com.adobe.csxs.events.Ai2Vlt.LoadHooks"

// Events we dispatch
#define EVENT_UPDATE_SELECTION			"com.adobe.csxs.events.Ai2Vlt.UpdateSelection"
#define EVENT_UPDATE_HOOK_FILE			"com.adobe.csxs.events.Ai2Vlt.UpdateHookFile"

static ai::UnicodeString getRelativePath(ai::FilePath base, const ai::FilePath & file)
{
	ai::UnicodeString fileStr = file.GetFullPath();
	ai::UnicodeString relativePath("");
	while (fileStr.find(base.GetDirectory()) != 0)
	{
		static const char s_back[3] = { '.', '.', ai::FilePath::GetDelimiter() };
		base = base.GetParent();
		relativePath.append(ai::UnicodeString(s_back));
	}

	relativePath.append(fileStr.substr(base.GetDirectory().length()));

	return relativePath;
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
	return result;
}

csxs::event::EventErrorCode Ai2VltPanelController::RemoveEventListeners()
{
	csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
	result = htmlPPLib.RemoveEventListener(EVENT_TYPE_PANEL_READY, PanelReadyFunc, this);
	if (result != csxs::event::kEventErrorCode_Success) return result;
	result = htmlPPLib.RemoveEventListener(EVENT_TYPE_LOAD_HOOKS, LoadHooksFunc, this);
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
			ai::FilePath const hookFile = ai::FilePath(ai::UnicodeString(hookFileEntry));
			std::string hookFileName = hookFile.GetFileName().as_Platform();
			csxs::event::Event event = { EVENT_UPDATE_HOOK_FILE,
				csxs::event::kEventScope_Application,
				ILST_APPID,
				nullptr,
				hookFileName.c_str() };

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
	filters.AddFilter(ai::UnicodeString("XML"), ai::UnicodeString("*.xml"));
	ai::FilePath hookFile = aiFile;
	err = sAIUser->GetFileDialog(ai::UnicodeString("Choose hook file"), &filters, hookFile);
	if (err) return;
	const ai::UnicodeString path = getRelativePath(aiFile, hookFile);

	AIDictKey key = sAIDictionary->Key("violetHookFile");
	err = sAIDictionary->SetStringEntry(m_docDictionary, key, path.as_Platform().c_str());
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

void Ai2VltPanelController::SetArt(AIArtHandle art)
{
	if (art != nullptr)
	{
		ASBoolean isDefaultName;
		ai::UnicodeString artName;
		short type;
		sAIArt->GetArtType(art, &type);
		sAIArt->GetArtName(art, artName, &isDefaultName);

		switch (type)
		{
		case kPathArt:
			if (isShape(art, "Rectangle"))
				DispatchUpdateEvent("Rect/Btn");
			else
				DispatchUpdateEvent("");
			break;
		case kTextFrameArt:
			DispatchUpdateEvent("Text");
			break;
		case kSymbolArt:
			DispatchUpdateEvent("Symbol");
			break;
		default:
			DispatchUpdateEvent("");
			break;
		}
	}
	else
		DispatchUpdateEvent("");
}

csxs::event::EventErrorCode Ai2VltPanelController::DispatchUpdateEvent(const std::string & payload)
{
	csxs::event::Event event = { EVENT_UPDATE_SELECTION,
		csxs::event::kEventScope_Application,
		ILST_APPID,
		nullptr,
		payload.c_str() };

	return htmlPPLib.DispatchEvent(&event);
}