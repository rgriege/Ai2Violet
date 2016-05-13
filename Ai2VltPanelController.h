#ifndef __AI2VLTPANELCONTROLLER_H__
#define __AI2VLTPANELCONTROLLER_H__

#include "SDKPlugPlug.h"
#include "IllustratorSDK.h"
#include "HtmlUIController.h"
#include "Ai2VltPlugin.h"

class Ai2VltPanelController final : public HtmlUIController
{
public:

	explicit Ai2VltPanelController(SPPluginRef plugin);
	Ai2VltPanelController(const Ai2VltPanelController &) = delete;
	Ai2VltPanelController & operator=(const Ai2VltPanelController &) = delete;
	virtual ~Ai2VltPanelController() override;

	virtual csxs::event::EventErrorCode RegisterCSXSEventListeners() override;
	virtual csxs::event::EventErrorCode RemoveEventListeners() override;
	virtual ASErr SendData() override { return kNoErr; }
	virtual void ParseData(const char *) override {}

	void CurrentDocumentChanged();
	void SendHooksToPanel();
	void LoadHooks();

	void ArtSelectionChanged();
	void SetArt(AIArtHandle art);

	void SelectHook(const char * hook);
	void SelectParam(const char * data);

public:

	const SPPluginRef m_pluginRef;

private:

	AIDictionaryRef m_docDictionary;
};

#endif
