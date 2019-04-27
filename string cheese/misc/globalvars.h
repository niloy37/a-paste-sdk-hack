#pragma once
#include "../sdk/c_input_system.h"
#include "../sdk/c_base_entity.h"
#include "../sdk/c_user_cmd.h"
namespace ap::g
{
	extern ap::sdk::c_user_cmd*		mango_cmd;
	extern ap::sdk::c_base_entity*	mango_local;
	extern std::uintptr_t			u_random_seed;
	extern ap::vec3f				original_view;
	extern bool						b_send_packet;
	extern int						target_index;
	extern ap::vec3f				enemy_eye_angles[65];
	extern ap::vec3f				aimbot_hitbox[65][28];
	extern ap::vec3f				real_angle;
	extern ap::vec3f				fake_angle;
	extern bool						shot[65];
	extern bool						hit[65];
	extern int						missed_shots[65];
}