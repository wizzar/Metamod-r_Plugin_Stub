#include <extdll.h>
#include <meta_api.h>
#include "interface.cpp" // Needed definition for Sys_GetModuleHandle.

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
		LOG_CONSOLE(PLID, "Failed to locate engine module\n");
		return false;
	}

	CreateInterfaceFn ifaceFactory = Sys_GetFactory(engineModule);
	if (!ifaceFactory) {
		LOG_CONSOLE(PLID, "Failed to locate interface factory in engine module\n");
		return false;
	}

	int retCode = 0;
	g_RehldsApi = (IRehldsApi*)ifaceFactory(VREHLDS_HLDS_API_VERSION, &retCode);
	if (!g_RehldsApi) {
		LOG_CONSOLE(PLID, "Failed to create interface from interface factory from engine module\n");
		return false;
	}

	int majorVersion = g_RehldsApi->GetMajorVersion();
	int minorVersion = g_RehldsApi->GetMinorVersion();

	if (majorVersion != REHLDS_API_VERSION_MAJOR)
	{
		LOG_CONSOLE(PLID, "[%s]: REHLDS Api major version mismatch. Expected: %d. Found: %d\n", Plugin_info.logtag, REHLDS_API_VERSION_MAJOR, majorVersion);

		// need to notify that it is necessary to update the ReHLDS.
		if (majorVersion < REHLDS_API_VERSION_MAJOR)
		{
			LOG_CONSOLE(PLID, "[%s]: Please update the ReHLDS up to a major version API >= %d\n", Plugin_info.logtag, REHLDS_API_VERSION_MAJOR);
		}
		else if (majorVersion > REHLDS_API_VERSION_MAJOR)
		{
			// need to notify that it is necessary to update the module.

			LOG_CONSOLE(PLID, "[%s]: Please update the %s up to a major version API >= %d\n", Plugin_info.logtag, Plugin_info.logtag, majorVersion);
		}

		return false;
	}

	if (minorVersion < REHLDS_API_VERSION_MINOR)
	{
		LOG_CONSOLE(PLID, "[%s]: REHLDS Api minor version mismatch. Expected: %d. Found: %d\n", Plugin_info.logtag, REHLDS_API_VERSION_MINOR, minorVersion);

		LOG_CONSOLE(PLID, "[%s]: Please update the ReHLDS up to a minor version API >= %d\n", Plugin_info.logtag, REHLDS_API_VERSION_MINOR);

		return false;
	}

	g_RehldsFuncs = g_RehldsApi->GetFuncs();
	g_RehldsHookchains = g_RehldsApi->GetHookchains();
	g_RehldsSvs = g_RehldsApi->GetServerStatic();
	g_RehldsData = g_RehldsApi->GetServerData();

	// message manager is available in "ReHLDS API" >= 3.14
		if (majorVersion >= 3 && minorVersion >= 14)
			rehlds_message_mngr_init();
		else
			g_RehldsMessageManager = nullptr;

	return true;
}

void rehlds_message_mngr_init()
{
	IMessageManager* messageManager = g_RehldsApi->GetMessageManager();

	int majorMessageMngrVersion = messageManager->getMajorVersion();
	int minorMessageMngrVersion = messageManager->getMinorVersion();

	if (majorMessageMngrVersion != MESSAGEMNGR_VERSION_MAJOR)
	{
		LOG_CONSOLE(PLID, "[%s]: ReHLDS MessageMngr API major version mismatch; expected %d.%d. Found: %d.%d\n", Plugin_info.logtag, MESSAGEMNGR_VERSION_MAJOR, MESSAGEMNGR_VERSION_MINOR, majorMessageMngrVersion, minorMessageMngrVersion);

		// need to notify that it is necessary to update the ReHLDS
		if (majorMessageMngrVersion < MESSAGEMNGR_VERSION_MAJOR)
		{
			LOG_CONSOLE(PLID, "[%s]: Please update ReHLDS to a newer version for the required MessageMngr API %d.%d\n", Plugin_info.logtag, MESSAGEMNGR_VERSION_MAJOR, MESSAGEMNGR_VERSION_MINOR);
		}

		// need to notify that it is necessary to update the module
		else if (majorMessageMngrVersion > MESSAGEMNGR_VERSION_MAJOR)
		{
			LOG_CONSOLE(PLID, "[%s]: Please update the %s to a newer version for the required MessageMngr API %d.%d\n", Plugin_info.logtag, Plugin_info.logtag, MESSAGEMNGR_VERSION_MAJOR, MESSAGEMNGR_VERSION_MINOR);
		}

		return;
	}

	if (minorMessageMngrVersion < MESSAGEMNGR_VERSION_MINOR)
	{
		LOG_CONSOLE(PLID, "[%s]: ReHLDS MessageMngr API minor version mismatch; expected at least %d.%d, real %d.%d\n", Plugin_info.logtag, MESSAGEMNGR_VERSION_MAJOR, MESSAGEMNGR_VERSION_MINOR, majorMessageMngrVersion, minorMessageMngrVersion);
		LOG_CONSOLE(PLID, "[%s]: Please update ReHLDS to a newer version for the required MessageMngr API %d.%d\n", Plugin_info.logtag, MESSAGEMNGR_VERSION_MAJOR, MESSAGEMNGR_VERSION_MINOR);
		return;
	}

	g_RehldsMessageManager = messageManager;
}
