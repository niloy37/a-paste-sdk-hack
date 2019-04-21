#pragma once
#include "../misc/globalvars.h"
namespace ap::g
{
	ap::sdk::c_user_cmd*	mango_cmd = nullptr;
	ap::sdk::c_base_entity* mango_local = nullptr;
	std::uintptr_t			u_random_seed = NULL;
	ap::vec3f				original_view;
	bool					b_send_packet;
	int						target_index = -1;
	ap::vec3f				enemy_eye_angles[65];
	ap::vec3f				aimbot_hitbox[65][28];
	ap::vec3f				real_angle;
	ap::vec3f				fake_angle;
	bool					shot[65];
	bool					hit[65];
	int						missed_shots[65];
}