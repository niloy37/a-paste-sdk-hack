#pragma once
// includes 
#include "../sdk/c_user_cmd.h"


#define backtrackspace namespace


backtrackspace ap::features::backtrack {
	void on_create_move(ap::sdk::c_user_cmd* mango_cmd);
	void on_paint_traverse();
	bool is_valid_backtrack_tick(float simtime);
}

