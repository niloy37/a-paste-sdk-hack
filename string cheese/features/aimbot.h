#pragma once
#include "../misc/vector.h"
#include "../sdk/c_base_entity.h"
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