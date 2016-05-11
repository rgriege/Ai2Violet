#include "IllustratorSDK.h"
#include "Ai2VltPanelController.h"
#include "AppContext.hpp"
#include "SDKErrors.h"
#include "Plugin.hpp"
#include "Utility.h"
#include "ezxml/ezxml.h"

#define AI2VLT_UI_EXTENSION				"com.bf.Ai2VltUi"
#define ILST_APPID						"ILST"

// Events we listen out for
#define EVENT_TYPE_PANEL_READY			"com.adobe.csxs.events.Ai2Vlt.PanelReady"

// Events we dispatch
#define EVENT_UPDATE_SELECTION			"com.adobe.csxs.events.Ai2Vlt.UpdateSelection"


static void PanelReadyFunc(const csxs::event::Event* const eventParam, void* const context)
{
	Ai2VltPanelController *ai2vltPanelController = (Ai2VltPanelController *)context;
	if (ai2vltPanelController == nullptr || eventParam == nullptr)
		return;

	ai2vltPanelController->ArtSelectionChanged();
}

Ai2VltPanelController::Ai2VltPanelController(void) :
	HtmlUIController(AI2VLT_UI_EXTENSION)
{
}

csxs::event::EventErrorCode Ai2VltPanelController::RegisterCSXSEventListeners()
{
	return htmlPPLib.AddEventListener(EVENT_TYPE_PANEL_READY, PanelReadyFunc, this);
}

csxs::event::EventErrorCode Ai2VltPanelController::RemoveEventListeners()
{
	return htmlPPLib.RemoveEventListener(EVENT_TYPE_PANEL_READY, PanelReadyFunc, this);
}

void Ai2VltPanelController::ArtSelectionChanged()
{
	ai::int32 artCount = 0;
	AIArtHandle **artStore = NULL;
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

csxs::event::EventErrorCode Ai2VltPanelController::DispatchUpdateEvent(const string& payload)
{
	csxs::event::Event event = { EVENT_UPDATE_SELECTION,
		csxs::event::kEventScope_Application,
		ILST_APPID,
		NULL,
		payload.c_str() };

	return htmlPPLib.DispatchEvent(&event);
}