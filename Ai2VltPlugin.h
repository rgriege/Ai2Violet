#ifndef __AI2VLTPLUGIN_H__
#define __AI2VLTPLUGIN_H__

#include "IllustratorSDK.h"
#include "Plugin.hpp"
#include "Ai2VltSuites.h"
#include "Ai2VltPanelController.h"
#include "SDKDef.h"
#include "SDKAboutPluginsHelper.h"

Plugin* AllocatePlugin(SPPluginRef pluginRef);
void FixupReload(Plugin* plugin);

// TODO(rgriege): why is this necessary?
class Ai2VltPanelController;

class Ai2VltPlugin final : public Plugin
{
public:

	Ai2VltPlugin(SPPluginRef pluginRef);
	Ai2VltPlugin(const Ai2VltPlugin &) = delete;
	Ai2VltPlugin & operator=(const Ai2VltPlugin &) = delete;
	virtual ~Ai2VltPlugin() = default;

	/**	Restores state of Ai2VltPlugin during reload. */
	FIXUP_VTABLE_EX(Ai2VltPlugin, Plugin);

private:

	virtual ASErr Message(char* caller, char* selector, void* message) override;
	virtual ASErr Notify(AINotifierMessage* message) override;
	virtual ASErr StartupPlugin(SPInterfaceMessage* message) override;
	virtual ASErr PostStartupPlugin() override;
	virtual ASErr ShutdownPlugin(SPInterfaceMessage* message) override;
	virtual ASErr GoMenuItem(AIMenuMessage* message) override;
	virtual ASErr GoFileFormat(AIFileFormatMessage* message) override;

	ASErr AddMenus(SPInterfaceMessage* message);
	ASErr AddFileFormats(SPInterfaceMessage* message);

private:

	AIFileFormatHandle m_fileFormat;
	AIMenuItemHandle m_aboutMenu;
	AIMenuItemHandle m_panelMenu;
	std::unique_ptr<Ai2VltPanelController> m_panelController;
	AINotifierHandle m_registerEventNotifier;
	AINotifierHandle m_artSelectionChangedNotifier;
	AINotifierHandle m_documentChangedNotifier;
	AINotifierHandle m_documentClosedNotifier;
};

#endif
