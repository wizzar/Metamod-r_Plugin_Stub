#include <extdll.h>
#include <meta_api.h>

/*
* Functions on this file are called by Metamod here https://github.com/rehlds/Metamod-R/blob/273618c684807a4b3809b23d2fd7a3353b46a5da/metamod/src/mplugin.cpp#L875.
* To use them, they must be passed to Metamod via META_FUNCTIONS defined in meta_api.cpp.
* There, we declare which functions the plugin catches.
*
* In these functions we give Metamod the functions
* we want it to call when something happens.
* You will notice that the "_Post" and regular corresponding
* structs have the same functions, because you can hook them
* before or after they are processed.
* 
*
* Example:
* Setting g_DllFunctionTable_Post.pfnServerActivate to a function
* will caused it to be called "POST", i.e., after the engine calls pfnServerActivate.
*/

DLL_FUNCTIONS g_DllFunctionTable =
{
	NULL,					// pfnGameInit
	NULL,					// pfnSpawn
	NULL,					// pfnThink
	NULL,					// pfnUse
	NULL,					// pfnTouch
	NULL,					// pfnBlocked
	NULL,					// pfnKeyValue
	NULL,					// pfnSave
	NULL,					// pfnRestore
	NULL,					// pfnSetAbsBox
	NULL,					// pfnSaveWriteFields
	NULL,					// pfnSaveReadFields
	NULL,					// pfnSaveGlobalState
	NULL,					// pfnRestoreGlobalState
	NULL,					// pfnResetGlobalState
	NULL,					// pfnClientConnect
	NULL,					// pfnClientDisconnect
	NULL,					// pfnClientKill
	NULL,					// pfnClientPutInServer
	NULL,					// pfnClientCommand
	NULL,					// pfnClientUserInfoChanged
	NULL,					// pfnServerActivate
	NULL,					// pfnServerDeactivate
	NULL,					// pfnPlayerPreThink
	NULL,					// pfnPlayerPostThink
	NULL,					// pfnStartFrame
	NULL,					// pfnParmsNewLevel
	NULL,					// pfnParmsChangeLevel
	NULL,					// pfnGetGameDescription
	NULL,					// pfnPlayerCustomization
	NULL,					// pfnSpectatorConnect
	NULL,					// pfnSpectatorDisconnect
	NULL,					// pfnSpectatorThink
	NULL,					// pfnSys_Error
	NULL,					// pfnPM_Move
	NULL,					// pfnPM_Init
	NULL,					// pfnPM_FindTextureType
	NULL,					// pfnSetupVisibility
	NULL,					// pfnUpdateClientData
	NULL,					// pfnAddToFullPack
	NULL,					// pfnCreateBaseline
	NULL,					// pfnRegisterEncoders
	NULL,					// pfnGetWeaponData
	NULL,					// pfnCmdStart
	NULL,					// pfnCmdEnd
	NULL,					// pfnConnectionlessPacket
	NULL,					// pfnGetHullBounds
	NULL,					// pfnCreateInstancedBaselines
	NULL,					// pfnInconsistentFile
	NULL,					// pfnAllowLagCompensation
};

DLL_FUNCTIONS g_DllFunctionTable_Post =
{
	NULL,					// pfnGameInit
	NULL,					// pfnSpawn
	NULL,					// pfnThink
	NULL,					// pfnUse
	NULL,					// pfnTouch
	NULL,					// pfnBlocked
	NULL,					// pfnKeyValue
	NULL,					// pfnSave
	NULL,					// pfnRestore
	NULL,					// pfnSetAbsBox
	NULL,					// pfnSaveWriteFields
	NULL,					// pfnSaveReadFields
	NULL,					// pfnSaveGlobalState
	NULL,					// pfnRestoreGlobalState
	NULL,					// pfnResetGlobalState
	NULL,					// pfnClientConnect
	NULL,					// pfnClientDisconnect
	NULL,					// pfnClientKill
	NULL,					// pfnClientPutInServer
	NULL,					// pfnClientCommand
	NULL,					// pfnClientUserInfoChanged
	NULL,					// pfnServerActivate
	NULL,					// pfnServerDeactivate
	NULL,					// pfnPlayerPreThink
	NULL,					// pfnPlayerPostThink
	NULL,					// pfnStartFrame
	NULL,					// pfnParmsNewLevel
	NULL,					// pfnParmsChangeLevel
	NULL,					// pfnGetGameDescription
	NULL,					// pfnPlayerCustomization
	NULL,					// pfnSpectatorConnect
	NULL,					// pfnSpectatorDisconnect
	NULL,					// pfnSpectatorThink
	NULL,					// pfnSys_Error
	NULL,					// pfnPM_Move
	NULL,					// pfnPM_Init
	NULL,					// pfnPM_FindTextureType
	NULL,					// pfnSetupVisibility
	NULL,					// pfnUpdateClientData
	NULL,					// pfnAddToFullPack
	NULL,					// pfnCreateBaseline
	NULL,					// pfnRegisterEncoders
	NULL,					// pfnGetWeaponData
	NULL,					// pfnCmdStart
	NULL,					// pfnCmdEnd
	NULL,					// pfnConnectionlessPacket
	NULL,					// pfnGetHullBounds
	NULL,					// pfnCreateInstancedBaselines
	NULL,					// pfnInconsistentFile
	NULL,					// pfnAllowLagCompensation
};

NEW_DLL_FUNCTIONS g_NewDllFunctionTable =
{
	NULL,					//! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its destructor.
	NULL,					//! pfnGameShutdown()
	NULL,					//! pfnShouldCollide()
	NULL,					//! pfnCvarValue()
	NULL,					//! pfnCvarValue2()
};

NEW_DLL_FUNCTIONS g_NewDllFunctionTable_Post =
{
	NULL,					//! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its destructor.
	NULL,					//! pfnGameShutdown()
	NULL,					//! pfnShouldCollide()
	NULL,					//! pfnCvarValue()
	NULL,					//! pfnCvarValue2()
};


C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
	if (!pFunctionTable) {
		// ALERT only shows up when HLDS ahas "log on".
		// Let's go ahead and log to console to make debugging easier.
		//ALERT(at_logged, "%s called with null pFunctionTable", __FUNCTION__);
		LOG_CONSOLE(PLID, "%s called with null pFunctionTable", __FUNCTION__);
		return FALSE;
	}

	if (*interfaceVersion != INTERFACE_VERSION) {
		// ALERT only shows up when HLDS ahas "log on".
		// Let's go ahead and log to console to make debugging easier.
		//ALERT(at_logged, "%s version mismatch; Expected: %d; Found: %d", __FUNCTION__, *interfaceVersion, INTERFACE_VERSION);
		LOG_CONSOLE(PLID, "%s version mismatch; Expected: %d; Found: %d", __FUNCTION__, *interfaceVersion, INTERFACE_VERSION);
		*interfaceVersion = INTERFACE_VERSION;
		return FALSE;
	}

	memcpy(pFunctionTable, &g_DllFunctionTable, sizeof(DLL_FUNCTIONS));

	return TRUE;
}

C_DLLEXPORT int GetEntityAPI2_Post(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
	if (!pFunctionTable) {
		// ALERT only shows up when HLDS ahas "log on".
		// Let's go ahead and log to console to make debugging easier.
		LOG_CONSOLE(PLID, "%s called with null pFunctionTable", __FUNCTION__);
		return FALSE;
	}

	if (*interfaceVersion != INTERFACE_VERSION) {
		// ALERT only shows up when HLDS ahas "log on".
		// Let's go ahead and log to console to make debugging easier.
		LOG_CONSOLE(PLID, "%s version mismatch; Expected: %d; Found: %d", __FUNCTION__, *interfaceVersion, INTERFACE_VERSION);
		*interfaceVersion = INTERFACE_VERSION;
		return FALSE;
	}

	memcpy(pFunctionTable, &g_DllFunctionTable_Post, sizeof(DLL_FUNCTIONS));

	return TRUE;
}

C_DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion)
{
	if (!pNewFunctionTable) {
		// ALERT only shows up when HLDS ahas "log on".
		// Let's go ahead and log to console to make debugging easier.
		LOG_CONSOLE(PLID, "%s called with null pNewFunctionTable", __FUNCTION__);
		return FALSE;
	}

	if (*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION) {
		// ALERT only shows up when HLDS ahas "log on".
		// Let's go ahead and log to console to make debugging easier.
		LOG_CONSOLE(PLID, "%s version mismatch; Expected: %d; Found: %d", __FUNCTION__, *interfaceVersion, NEW_DLL_FUNCTIONS_VERSION);
		*interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
		return FALSE;
	}

	memcpy(pNewFunctionTable, &g_NewDllFunctionTable, sizeof(NEW_DLL_FUNCTIONS));
	return TRUE;
}

C_DLLEXPORT int GetNewDLLFunctions_Post(NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion)
{
	if (!pNewFunctionTable) {
		// ALERT only shows up when HLDS ahas "log on".
		// Let's go ahead and log to console to make debugging easier.
		LOG_CONSOLE(PLID, "%s called with null pNewFunctionTable", __FUNCTION__);
		return FALSE;
	}

	if (*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION) {
		// ALERT only shows up when HLDS ahas "log on".
		// Let's go ahead and log to console to make debugging easier.
		LOG_CONSOLE(PLID, "%s version mismatch; Expected: %d; Found: %d", __FUNCTION__, *interfaceVersion, NEW_DLL_FUNCTIONS_VERSION);
		*interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
		return FALSE;
	}

	memcpy(pNewFunctionTable, &g_NewDllFunctionTable_Post, sizeof(NEW_DLL_FUNCTIONS));
	return TRUE;
}
