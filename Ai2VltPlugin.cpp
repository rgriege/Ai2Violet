#include "IllustratorSDK.h"
#include "Ai2VltExporter.h"
#include "Ai2VltPlugin.h"

#ifdef MAC_ENV
    #include <ApplicationServices/ApplicationServices.h>
#endif

#ifdef WIN_ENV
	#include "shellapi.h"
#endif 

static void CStrToPStr(char *s, ai::UnicodeString::size_type len);


Plugin* AllocatePlugin(SPPluginRef pluginRef)
{
	return new Ai2VltPlugin(pluginRef);
}

void FixupReload(Plugin* plugin)
{
	Ai2VltPlugin::FixupVTable((Ai2VltPlugin*) plugin);
}


Ai2VltPlugin::Ai2VltPlugin(SPPluginRef pluginRef) :
	Plugin(pluginRef)
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

ASErr Ai2VltPlugin::StartupPlugin(SPInterfaceMessage* message)
{
	ASErr error = kNoErr;
	error = Plugin::StartupPlugin(message);
    if (error)
		return error;
	error = this->AddMenus(message);
	if (error)
		return error;
	error = this->AddFileFormats(message);
	
	return error;
}

ASErr Ai2VltPlugin::GoMenuItem(AIMenuMessage* message)
{
	if (message->menuItem == this->m_aboutPluginMenu)
	{
		SDKAboutPluginsHelper aboutPluginsHelper;
		aboutPluginsHelper.PopAboutBox(message, "Ai->Violet Export Plug-In", "Developed for the Violet engine of Building Forge, LLC");
	}	
	return kNoErr;
}


ASErr Ai2VltPlugin::AddMenus(SPInterfaceMessage* message)
{
	SDKAboutPluginsHelper aboutPluginsHelper;
	return aboutPluginsHelper.AddAboutPluginsMenuItem(message, 
		"AboutVioletPluginsGroupName", 
		ai::UnicodeString("About Violet Plug-Ins"),
		"Ai->Violet...", 
		&this->m_aboutPluginMenu);
}

ASErr Ai2VltPlugin::AddFileFormats(SPInterfaceMessage* message) 
{
	PlatformAddFileFormatData affd;
	char pstrCanvas[kMaxStringLength] = "Violet";
	
	CStrToPStr(pstrCanvas, kMaxStringLength);
	affd.title = (unsigned char*)pstrCanvas;
	affd.titleOrder = 0;
	affd.extension = "vsvg";
	
	return sAIFileFormat->AddFileFormat(message->d.self, "Violet",
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

void CStrToPStr(char *s, ai::UnicodeString::size_type len)
{
	const ai::UnicodeString sAsUnicode((const char*)s);
	ai::PStr sAsPStr((unsigned char*) s);
	sAsUnicode.getToBuffer(sAsPStr, len, kAIUTF8CharacterEncoding );
}
