#include <extdll.h>
#include <meta_api.h>
#include "rehlds_api_plugin.h"
#include "regame_api_plugin.h"

// Define meta_api global externs.
meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;

plugin_info_t Plugin_info =
{
	META_INTERFACE_VERSION,		// ifvers
	"Example plugin",			// name
	"0.1",						// version
	__DATE__,					// date
	"Author",					// author
	"http://",					// url
	"EXAMPLE",					// logtag
	PT_ANYTIME,					// (when) loadable
	PT_ANYTIME,					// (when) unloadable
};

// Whatever we don't use, just set it to NULL.
// Must provide at least one function returning a standard HL function table,
// from either the following standard HLSDK functions.
// These are defined in the engine_api.cpp and dllapi.cpp files.
META_FUNCTIONS gMetaFunctionTable =
{
	NULL,						//            pfnGetEntityAPI	HL SDK; called before game DLL;
	NULL,						//       pfnGetEntityAPI_Post	META; called after game DLL;
	GetEntityAPI2,				//           pfnGetEntityAPI2	HL SDK2; called before game DLL;
	GetEntityAPI2_Post,			//      pfnGetEntityAPI2_Post	META; called after game DLL;
	GetNewDLLFunctions,			//      pfnGetNewDLLFunctions	HL SDK2; called before game DLL;
	GetNewDLLFunctions_Post,	// pfnGetNewDLLFunctions_Post	META; called after game DLL;
	GetEngineFunctions,			//      pfnGetEngineFunctions	META; called before HL engine;
	GetEngineFunctions_Post,	// pfnGetEngineFunctions_Post	META; called after HL engine;
};

// This function is optional.
// Called after GiveFnptrsToDll.
C_DLLEXPORT void Meta_Init(void) { }

// Metamod requesting info about this plugin:
// 
// interfaceVersion		(given)		interface version (META_INTERFACE_VERSION) Metamod is using;
//           plinfo		(requested)	struct with info about plugin that we must provide to Metamod;
//   pMetaUtilFuncs		(given)		table of utility functions provided by Metamod;
C_DLLEXPORT int Meta_Query(char *interfaceVersion, plugin_info_t **plinfo, mutil_funcs_t *pMetaUtilFuncs)
{
	*plinfo = &Plugin_info;
	gpMetaUtilFuncs = pMetaUtilFuncs;
	return TRUE;
}

// Metamod attaching plugin to the server.
//
//             now		(given)		current phase, i.e., during map, during changelevel, or at startup;
//  pFunctionTable		(requested)	table of function tables this plugin catches;
//       pMGlobals		(given)		global vars from Metamod;
//   pGamedllFuncs		(given)		copy of function tables from game dll;
// https://github.com/rehlds/Metamod-R/blob/273618c684807a4b3809b23d2fd7a3353b46a5da/metamod/src/mplugin.cpp#L969
C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs)
{
	// Store global vars from metamod.
	if (!pMGlobals) {
		LOG_CONSOLE(PLID, "Meta_Attach called with null pMGlobals");
		return(FALSE);
	}
	gpMetaGlobals = pMGlobals;

	// Give metamod function tables this plugin catches.
	if (!pFunctionTable) {
		LOG_CONSOLE(PLID, "Meta_Attach called with null pFunctionTable");
		return(FALSE);
	}
	memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));

	// Store functions from game dll.
	gpGamedllFuncs = pGamedllFuncs;

	LOG_CONSOLE(PLID, "\n\n#####\n# Meta_Attach from plugin %s #\n#####\n\n", Plugin_info.name);

	// This is not plain metamod only.
	// Whole nother universe here.
	if (meta_init_rehlds_api())
		LOG_CONSOLE(PLID, "ReHLDS API successfully initialized.\n");

	if (regamedll_api_init())
		LOG_CONSOLE(PLID, "ReGameDLL API successfully initialized.\n");

	return TRUE;
}

// Metamod detaching plugin from the server.
// 
//    now	(given)	current phase, i.e., during map, during changelevel, or at startup;
// reason	(given)	why detaching (refresh, console unload, forced unload, etc);
C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	regamedll_api_stop();
	return TRUE;
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