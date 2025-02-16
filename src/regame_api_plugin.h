#pragma once

#include <regamedll_api.h>

//#include <vector.h>
//#include <progdefs.h>

// Make them extern here so that if
// they are needed elsewhere then
// just this header needs to be included.
extern IReGameApi* g_ReGameApi;
extern const ReGameFuncs_t* g_ReGameFuncs;
extern IReGameHookchains* g_ReGameHookchains;
extern CGameRules* g_pGameRules;

bool regamedll_api_init();
bool regamedll_api_stop();

CGameRules* InstallGameRules(IReGameHook_InstallGameRules* chain);

// Hook function example.
//Vector& ReGameDLL_CBaseEntity_FireBullets3(IReGameHook_CBaseEntity_FireBullets3* chain, CBaseEntity* pEntity, Vector& vecSrc, Vector& vecDirShooting, float vecSpread, float flDistance, int iPenetration, int iBulletType, int iDamage, float flRangeModifier, entvars_t* pevAttacker, bool bPistol, int shared_rand);
