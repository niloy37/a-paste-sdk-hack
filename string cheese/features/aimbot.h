#pragma once
#include "../misc/vector.h"
#include "../sdk/c_base_entity.h"
#include "../sdk/c_user_cmd.h"
#include "../menu.h"
#include "../misc/math.h"
namespace ap::features::aimbot {
	
	void no_recoil();
	void on_create_move(sdk::c_user_cmd* mango_cmd);
}