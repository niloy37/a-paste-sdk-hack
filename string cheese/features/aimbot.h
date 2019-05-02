#pragma once
#include "../misc/singleton.h"
#include "../sdk/c_base_entity.h"
#include "../sdk/c_base_weapon.h"
#include <deque>
#include "../sdk/c_user_cmd.h"

namespace ap::features::aimbot {
	class ragebot : public singleton <ragebot>{
	public:
		void run(ap::sdk::c_user_cmd* cmd);
		void no_recoil(ap::sdk::c_user_cmd* cmd);
		void begin(ap::sdk::c_user_cmd* cmd);
		vec3f GetHitboxPosition(ap::sdk::c_base_entity* pEntity, int Hitbox);
		vec3f head_hitbox(ap::sdk::c_base_entity* e);
	};
}
