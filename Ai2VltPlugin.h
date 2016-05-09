#ifndef __AI2VLTPLUGIN_H__
#define __AI2VLTPLUGIN_H__

#include "IllustratorSDK.h"
#include "Plugin.hpp"
#include "Ai2VltSuites.h"
#include "SDKDef.h"
#include "SDKAboutPluginsHelper.h"

#define kMaxStringLength 256

/**	Creates a new Ai2VltPlugin.
	@param pluginRef IN unique reference to this plugin.
	@return pointer to new Ai2VltPlugin.
*/
Plugin* AllocatePlugin(SPPluginRef pluginRef);

/**	Reloads the Ai2VltPlugin class state when the plugin is 
	reloaded by the application.
	@param plugin IN pointer to plugin being reloaded.
*/
void FixupReload(Plugin* plugin);

/**	Provides a plugin which demonstrates adding new file formats to open and save to.
*/
class Ai2VltPlugin final : public Plugin
{
public:

	Ai2VltPlugin(SPPluginRef pluginRef);
	virtual ~Ai2VltPlugin() = default;

	/**	Restores state of Ai2VltPlugin during reload. */
	FIXUP_VTABLE_EX(Ai2VltPlugin, Plugin);

private:

	/** Calls Plugin::Message and handles any errors returned.
		@param caller IN sender of the message.
		@param selector IN nature of the message.
		@param message IN pointer to plugin and call information.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr Message(char* caller, char* selector, void *message);

	/**	Calls Plugin::Startup and initialisation functions, such as 
		AddMenus and AddNotifiers.
		@param message IN pointer to plugin and call information.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr StartupPlugin(SPInterfaceMessage* message);

	/**	Performs actions required for menu item selected.
		@param message IN pointer to plugin and call information.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr GoMenuItem(AIMenuMessage* message);
	
	/**	Performs actions required for file format selected.
		@param message IN pointer to plugin and call information.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr GoFileFormat(AIFileFormatMessage* message);

	/**	Adds the menu items for this plugin to the application UI.
	@param message IN pointer to plugin and call information.
	@return kNoErr on success, other ASErr otherwise.
	*/
	ASErr AddMenus(SPInterfaceMessage* message);

	/**	Adds the file formats for this plugin to the application.
	@param message IN pointer to plugin and call information.
	@return kNoErr on success, other ASErr otherwise.
	*/
	ASErr AddFileFormats(SPInterfaceMessage* message);

private:

	AIFileFormatHandle m_fileFormat;
	AIMenuItemHandle m_aboutPluginMenu;
};

#endif
