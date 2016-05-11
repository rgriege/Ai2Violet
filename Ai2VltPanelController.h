#ifndef __AI2VLTPANELCONTROLLER_H__
#define __AI2VLTPANELCONTROLLER_H__

#include "SDKPlugPlug.h"
#include "IllustratorSDK.h"
#include "HtmlUIController.h"
#include "Ai2VltPlugin.h"

class Ai2VltPanelController final : public HtmlUIController
{
public:

	Ai2VltPanelController();
	virtual ~Ai2VltPanelController() override = default;

	virtual csxs::event::EventErrorCode RegisterCSXSEventListeners() override;
	virtual csxs::event::EventErrorCode RemoveEventListeners() override;
	virtual ASErr SendData() override { return kNoErr; }
	virtual void ParseData(const char *) override {}

	void ArtSelectionChanged();
	void SetArt(AIArtHandle art);

private:

	csxs::event::EventErrorCode DispatchUpdateEvent(const string& payload);
};

#endif
