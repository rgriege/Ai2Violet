#include "IllustratorSDK.h"
#include "Ai2VltExporter.h"
#include "Ai2VltPlugin.h"
#include "AICSXS.h"

static void stringToPascal(char *s, ai::UnicodeString::size_type len);


Plugin* AllocatePlugin(SPPluginRef pluginRef)
{
	return new Ai2VltPlugin(pluginRef);
}

void FixupReload(Plugin* plugin)
{
	Ai2VltPlugin::FixupVTable((Ai2VltPlugin*) plugin);
}


Ai2VltPlugin::Ai2VltPlugin(SPPluginRef pluginRef) :
	Plugin(pluginRef),
	m_fileFormat(nullptr),
	m_aboutMenu(nullptr),
	m_panelMenu(nullptr),
	m_panelController(nullptr),
	m_registerEventNotifier(nullptr),
	m_artSelectionChangedNotifier(nullptr)
{
	strncpy(fPluginName, "Ai2Vlt", kMaxStringLength);
}

ASErr Ai2VltPlugin::Message(char* caller, char* selector, void *message) 
{
	ASErr error = kNoErr;

	try
	{
		error = Plugin::Message(caller, selector, message);
	}
	catch (ai::Error& ex)
	{
		error = ex;
	}
	catch (...) 
	{
		error = kCantHappenErr;
	}
	if (error)
	{
		if (error == kUnhandledMsgErr)
			error = kNoErr;
		else
			Plugin::ReportError(error, caller, selector, message);
	}	
	return error;
}

ASErr Ai2VltPlugin::Notify(AINotifierMessage* message)
{
	if (message->notifier == m_registerEventNotifier)
	{
		m_panelController->RegisterCSXSEventListeners();
	}
	else if (message->notifier == m_artSelectionChangedNotifier)
	{
		m_panelController->ArtSelectionChanged();
	}
	else if (message->notifier == m_documentChangedNotifier)
	{
		m_panelController->CurrentDocumentChanged();
		m_panelController->ArtSelectionChanged();
	}
	else if (message->notifier == m_documentClosedNotifier)
	{
		m_panelController->CurrentDocumentChanged();
		m_panelController->ArtSelectionChanged();
	}
	return kNoErr;
}

ASErr Ai2VltPlugin::StartupPlugin(SPInterfaceMessage* message)
{
	ASErr error = kNoErr;
	error = Plugin::StartupPlugin(message);
    if (error) return error;
	m_panelController = std::make_unique<Ai2VltPanelController>(fPluginRef);
	error = this->AddMenus(message);
	if (error) return error;
	error = this->AddFileFormats(message);
	if (error) return error;
	// TODO(rgriege): use plugin name?
	error = sAINotifier->AddNotifier(fPluginRef, "Register Event Notify", kAICSXSPlugPlugSetupCompleteNotifier, &m_registerEventNotifier);
	if (error) return error;
	error = sAINotifier->AddNotifier(fPluginRef, fPluginName, kAIArtSelectionChangedNotifier, &m_artSelectionChangedNotifier);
	if (error) return error;
	error = sAINotifier->AddNotifier(fPluginRef, fPluginName, kAIDocumentChangedNotifier, &m_documentChangedNotifier);
	if (error) return error;
	error = sAINotifier->AddNotifier(fPluginRef, fPluginName, kAIDocumentClosedNotifier, &m_documentClosedNotifier);
	return error;
}

ASErr Ai2VltPlugin::ShutdownPlugin(SPInterfaceMessage* message)
{
	if (m_panelController != nullptr)
	{
		m_panelController->RemoveEventListeners();
		m_panelController->SetArt(nullptr);
		m_panelController.reset();
		Plugin::LockPlugin(false);
	}

	message->d.globals = nullptr;
	return Plugin::ShutdownPlugin(message);
}

ASErr Ai2VltPlugin::GoMenuItem(AIMenuMessage* message)
{
	if (message->menuItem == this->m_aboutMenu)
	{
		SDKAboutPluginsHelper aboutPluginsHelper;
		aboutPluginsHelper.PopAboutBox(message, "Ai->Violet Export Plug-In", "Developed for the Violet engine of Building Forge, LLC");
	}
	else if (message->menuItem == this->m_panelMenu)
	{
		m_panelController->LoadExtension();
	}
	return kNoErr;
}


ASErr Ai2VltPlugin::AddMenus(SPInterfaceMessage* message)
{
	ASErr error;
	SDKAboutPluginsHelper aboutPluginsHelper;
	error = aboutPluginsHelper.AddAboutPluginsMenuItem(message, 
		"AboutVioletPluginsGroupName", 
		ai::UnicodeString("About Violet Plug-Ins"),
		"Ai->Violet...", 
		&this->m_aboutMenu);
	if (error) return error;
	
	error = sAIMenu->AddMenuItemZString(message->d.self,
		"Violet Menu Item",
		kToolPalettesMenuGroup,
		ZREF("Violet"),
		kMenuItemWantsUpdateOption,
		&m_panelMenu);
	return error;
}

ASErr Ai2VltPlugin::AddFileFormats(SPInterfaceMessage* message) 
{
	PlatformAddFileFormatData affd;
	char pstrCanvas[kMaxStringLength] = "Violet SVG";
	
	stringToPascal(pstrCanvas, kMaxStringLength);
	affd.title = (unsigned char*)pstrCanvas;
	affd.titleOrder = 0;
	affd.extension = "vsvg";
	
	return sAIFileFormat->AddFileFormat(message->d.self, "Violet SVG",
		&affd, kFileFormatExport,
		&this->m_fileFormat, kNoExtendedOptions);
}

ASErr Ai2VltPlugin::GoFileFormat(AIFileFormatMessage* message) 
{
	char pathName[300];
	message->GetFilePath().GetFullPath().as_Roman(pathName, 300);
	
	if (message->option & kFileFormatExport)
		Ai2Vlt::exportSvg(pathName);
	
	return kNoErr;
}

void stringToPascal(char *s, ai::UnicodeString::size_type len)
{
	const ai::UnicodeString sAsUnicode((const char*)s);
	ai::PStr sAsPStr((unsigned char*) s);
	sAsUnicode.getToBuffer(sAsPStr, len, kAIUTF8CharacterEncoding );
}
