#pragma once
#include "../misc/singleton.h"
#include "../sdk/c_base_entity.h"
#include "../sdk/c_base_weapon.h"
#include <deque>
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_globals.h"
namespace ap::features::aimbot {
	void run(sdk::c_user_cmd* cmd);
	
}
