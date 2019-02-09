// includes 
#include <list>
#include <algorithm>
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"
#include "../sdk/c_client_state.h"
#include "../sdk/c_globals.h"
#include "../sdk/c_player_info.h"



namespace ap::features::backtrack {
	void on_create_move(ap::sdk::c_user_cmd* mango_cmd);
	void on_paint_traverse();
	bool is_valid_backtrack_tick(float simtime);
}

