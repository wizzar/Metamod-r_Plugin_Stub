#include "wrapper_meta_api.h"
#include "regame_api_plugin.h"

IReGameApi* g_ReGameApi;
const ReGameFuncs_t* g_ReGameFuncs;
IReGameHookchains* g_ReGameHookchains;
CGameRules* g_pGameRules = nullptr;

CGameRules* InstallGameRules(IReGameHook_InstallGameRules* chain)
{
	auto gamerules = chain->callNext();
	g_pGameRules = gamerules;
	return gamerules;
}

bool regamedll_api_init()
{
	const char* szGameDLLModule = gpMetaUtilFuncs->pfnGetGameInfo(PLID, GINFO_DLL_FULLPATH);
	if (!szGameDLLModule)
		return false;

	CSysModule* gameModule = Sys_GetModuleHandle(szGameDLLModule);
	if (!gameModule)
		return false;

	CreateInterfaceFn ifaceFactory = Sys_GetFactory(gameModule);
	if (!ifaceFactory)
		return false;

	int retCode = 0;
	g_ReGameApi = (IReGameApi*)ifaceFactory(VRE_GAMEDLL_API_VERSION, &retCode);
	if (!g_ReGameApi)
	{
		return false;
	}

	int majorVersion = g_ReGameApi->GetMajorVersion();
	int minorVersion = g_ReGameApi->GetMinorVersion();

	if (majorVersion != REGAMEDLL_API_VERSION_MAJOR)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: ReGameDLL API major version mismatch. Expected: %d. Found: %d\n", Plugin_info.logtag, REGAMEDLL_API_VERSION_MAJOR, majorVersion);

		// need to notify that it is necessary to update the ReGameDLL.
		if (majorVersion < REGAMEDLL_API_VERSION_MAJOR)
		{
			gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update the ReGameDLL up to a major version API >= %d\n", Plugin_info.logtag, REGAMEDLL_API_VERSION_MAJOR);
		}

		// need to notify that it is necessary to update the module.
		else if (majorVersion > REGAMEDLL_API_VERSION_MAJOR)
		{
			gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update the %s up to a major version API >= %d\n", Plugin_info.logtag, Plugin_info.logtag, majorVersion);
		}

		return false;
	}

	if (minorVersion < REGAMEDLL_API_VERSION_MINOR)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: ReGameDLL API minor version mismatch. Expected: %d. Found: %d\n", Plugin_info.logtag, REGAMEDLL_API_VERSION_MINOR, minorVersion);

		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update the ReGameDLL up to a minor version API >= %d\n", Plugin_info.logtag, REGAMEDLL_API_VERSION_MINOR);

		return false;
	}

	// Safe check CCSEntity API interface version
	if (!g_ReGameApi->BGetICSEntity(CSENTITY_API_INTERFACE_VERSION))
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Interface CCSEntity API version '%s' not found.\n", Plugin_info.logtag, CSENTITY_API_INTERFACE_VERSION);

		if (g_ReGameApi->BGetICSEntity("CSENTITY_API_INTERFACE_VERSION002"))
			gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update ReGameDLL to the latest version.\n", Plugin_info.logtag);
		else
			gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Please update ReAPI to the latest version.\n", Plugin_info.logtag);

		return false;
	}
	
	// Safe check GameRules API interface version
	if (!g_ReGameApi->BGetIGameRules(GAMERULES_API_INTERFACE_VERSION))
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s]: Interface GameRules API version '%s' not found.\n", Plugin_info.logtag, GAMERULES_API_INTERFACE_VERSION);

		return false;
	}

	g_ReGameFuncs = g_ReGameApi->GetFuncs();
	g_ReGameHookchains = g_ReGameApi->GetHookchains();

	g_ReGameHookchains->InstallGameRules()->registerHook(&InstallGameRules);

	return true;
}

bool regamedll_api_stop()
{
	if (g_pGameRules)
	{
		g_ReGameHookchains->InstallGameRules()->unregisterHook(InstallGameRules);
	}

	return true;
}

bool meta_init_regamedll_api()
{
	return regamedll_api_init();
}

bool meta_stop_regamedll_api()
{
	return regamedll_api_stop();
}