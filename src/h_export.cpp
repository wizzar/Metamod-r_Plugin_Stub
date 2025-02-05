#include <extdll.h>

enginefuncs_t g_engfuncs;
globalvars_t* gpGlobals;

// Receive engine function table and globals from the engine.
// This appears to be the _first_ DLL routine called by the engine,
// so we do some setup operations here.
//
// This function is called by GoldSrc engine
// when it is loading the game/mod dll (mp.dll for Counter-Strike).
// It is used to send engine functions to the game so that
// it can do what it wants with them, such as order a render,
// load a file, play a sound, whatever the game engine provides.
// GoldSrc was not made public by Valve, but you can get an
// idea of how it works here https://github.com/headcrab-junkyard/OGS/.
//
// Metamod will receive this call AS IF IT WAS the mod to be loaded,
// and will then sit between the engine and the actual game,
// allowing it to modify function calls etc.
// Metamod itself calls this function for each metamod plugin it loads as if it was the engine (https://github.com/rehlds/Metamod-R/blob/273618c684807a4b3809b23d2fd7a3353b46a5da/metamod/src/mplugin.cpp#L795).
//
// After this, we go on to the meta_api file.
C_DLLEXPORT void WINAPI GiveFnptrsToDll(enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals)
{
	memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
	gpGlobals = pGlobals;
}
