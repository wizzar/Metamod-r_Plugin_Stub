#include "wrapper_meta_api.h"
#include "interface.cpp" // needed definition for Sys_GetModuleHandle

#include "rehlds_api_plugin.h"

IRehldsApi* g_RehldsApi;
const RehldsFuncs_t* g_RehldsFuncs;
IRehldsHookchains* g_RehldsHookchains;
IRehldsServerStatic* g_RehldsSvs;
IRehldsServerData* g_RehldsData;
IMessageManager* g_RehldsMessageManager;

// Available in "ReHLDS API" >= 3.14.
void rehlds_message_mngr_init();

bool rehlds_api_init(CSysModule* engineModule)
{
	if (!engineModule) {
		gpMetaUtilFuncs->pfnLogConsole(PLID, "Failed to locate engine module\n");
		return false;
	}

	CreateInterfaceFn ifaceFactory = Sys_GetFactory(engineModule);
	if (!ifaceFactory) {
		gpMetaUtilFuncs->pfnLogConsole(PLID, "Failed to locate interface factory in engine module\n");
		return false;
	}

	int retCode = 0;
	g_RehldsApi = (IRehldsApi*)ifaceFactory(VREHLDS_HLDS_API_VERSION, &retCode);
	if (!g_RehldsApi) {
		return false;
	}

	int majorVersion = g_RehldsApi->GetMajorVersion();
	int minorVersion = g_RehldsApi->GetMinorVersion();

	if (majorVersion != REHLDS_API_VERSION_MAJOR)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: REHLDS Api major version mismatch. Expected: %d. Found: %d\n", Plugin_info.logtag, REHLDS_API_VERSION_MAJOR, majorVersion);

		// need to notify that it is necessary to update the ReHLDS.
		if (majorVersion < REHLDS_API_VERSION_MAJOR)
		{
			gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update the ReHLDS up to a major version API >= %d\n", Plugin_info.logtag, REHLDS_API_VERSION_MAJOR);
		}
		else if (majorVersion > REHLDS_API_VERSION_MAJOR)
		{
			// need to notify that it is necessary to update the module.

			gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update the %s up to a major version API >= %d\n", Plugin_info.logtag, Plugin_info.logtag, majorVersion);
		}

		return false;
	}

	if (minorVersion < REHLDS_API_VERSION_MINOR)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: REHLDS Api minor version mismatch. Expected: %d. Found: %d\n", Plugin_info.logtag, REHLDS_API_VERSION_MINOR, minorVersion);

		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update the ReHLDS up to a minor version API >= %d\n", Plugin_info.logtag, REHLDS_API_VERSION_MINOR);

		return false;
	}

	g_RehldsFuncs = g_RehldsApi->GetFuncs();
	g_RehldsHookchains = g_RehldsApi->GetHookchains();
	g_RehldsSvs = g_RehldsApi->GetServerStatic();
	g_RehldsData = g_RehldsApi->GetServerData();
	//g_RehldsMessageManager = nullptr;

	// message manager is available in "ReHLDS API" >= 3.14
		if (majorVersion >= 3 && minorVersion >= 14)
			rehlds_message_mngr_init();

	return true;
}

void rehlds_message_mngr_init()
{
	IMessageManager* messageManager = g_RehldsApi->GetMessageManager();

	int majorMessageMngrVersion = messageManager->getMajorVersion();
	int minorMessageMngrVersion = messageManager->getMinorVersion();

	if (majorMessageMngrVersion != MESSAGEMNGR_VERSION_MAJOR)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: ReHLDS MessageMngr API major version mismatch; expected %d.%d. Found: %d.%d\n", Plugin_info.logtag, MESSAGEMNGR_VERSION_MAJOR, MESSAGEMNGR_VERSION_MINOR, majorMessageMngrVersion, minorMessageMngrVersion);

		// need to notify that it is necessary to update the ReHLDS
		if (majorMessageMngrVersion < MESSAGEMNGR_VERSION_MAJOR)
		{
			gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update ReHLDS to a newer version for the required MessageMngr API %d.%d\n", Plugin_info.logtag, MESSAGEMNGR_VERSION_MAJOR, MESSAGEMNGR_VERSION_MINOR);
		}

		// need to notify that it is necessary to update the module
		else if (majorMessageMngrVersion > MESSAGEMNGR_VERSION_MAJOR)
		{
			gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update the %s to a newer version for the required MessageMngr API %d.%d\n", Plugin_info.logtag, Plugin_info.logtag, MESSAGEMNGR_VERSION_MAJOR, MESSAGEMNGR_VERSION_MINOR);
		}

		return;
	}

	if (minorMessageMngrVersion < MESSAGEMNGR_VERSION_MINOR)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: ReHLDS MessageMngr API minor version mismatch; expected at least %d.%d, real %d.%d\n", Plugin_info.logtag, MESSAGEMNGR_VERSION_MAJOR, MESSAGEMNGR_VERSION_MINOR, majorMessageMngrVersion, minorMessageMngrVersion);
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update ReHLDS to a newer version for the required MessageMngr API %d.%d\n", Plugin_info.logtag, MESSAGEMNGR_VERSION_MAJOR, MESSAGEMNGR_VERSION_MINOR);
		return;
	}

	g_RehldsMessageManager = messageManager;
}

bool meta_init_rehlds_api()
{
#ifdef _WIN32
	// Find the most appropriate module handle from a list of DLL candidates
	// Notes:
	// - "swds.dll" is the library Dedicated Server
	//
	//    Let's also attempt to locate the ReHLDS API in the client's library
	// - "sw.dll" is the client library for Software render, with a built-in listenserver
	// - "hw.dll" is the client library for Hardware render, with a built-in listenserver
	const char* dllNames[] = { "swds.dll", "hw.dll", "sw.dll" }; // List of DLL candidates to lookup for the ReHLDS API
	CSysModule* engineModule = NULL; // The module handle of the selected DLL
	for (const char* dllName : dllNames)
	{
		if (engineModule = Sys_GetModuleHandle(dllName))
			break; // gotcha
	}

#else
	CSysModule* engineModule = Sys_GetModuleHandle("engine_i486.so");
#endif

	if (!rehlds_api_init(engineModule)) {
		return false;
	}

	return true;
}
