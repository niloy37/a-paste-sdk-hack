#pragma once
#include <list>
#include <algorithm>
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"
#include "../sdk/c_client_state.h"
#include "../sdk/c_globals.h"
#include "../sdk/c_player_info.h"
#include "../features/backtrack.h"
#include "../misc/renderer.h"
#include "../sdk/c_cvar.h"
#include "../misc/variables.h"
#include "../misc/singleton.h"
#include "../sdk/c_trace.h"
#include "../sdk/c_base_weapon.h"
#include "../misc/globalvars.h"
#include "../features/autowall.h"

namespace ap::features::ragebot {

	matrix3x4_t Matrix[65][128];
	int Sequence;
	void OnCreateMove();
	int bestEntDmg;


	bool Backtrack[65];
	bool ShotBacktrack[65];
	ap::vec3f hit_scan(ap::sdk::c_base_entity* pEnt);
	bool hit_chance(ap::sdk::c_base_entity* pEnt, ap::sdk::c_base_weapon* pWeapon, ap::vec3f Angle, ap::vec3f Point, int chance);
	void auto_stop();
}