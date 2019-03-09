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
#include "../sdk/c_cvar.h"

namespace ap::features::ragebot {
	void drop_target();
	void run();
	void on_create_move();
	int target_index = -1;
	float best_distance = 8192.f;
	bool aimbotted_in_current_tick;
	bool fired_in_that_tick;
	float current_aim_simulationtime;
	vec3f current_aim_position;
	vec3f current_aim_player_origin;
	ap::sdk::c_base_entity* pTarget;
}