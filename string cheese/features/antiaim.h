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
#include "../features/backtrack.h"
#include "../misc/renderer.h"
#include "../sdk/c_cvar.h"



namespace ap::features::antiaim {

	void slide_walk(ap::sdk::c_user_cmd* mango_cmd) {

		if (mango_cmd->forwardmove > 0)
		{
			mango_cmd->buttons |= IN_BACK;
			mango_cmd->buttons &= ~IN_FORWARD;
		}

		if (mango_cmd->forwardmove < 0)
		{
			mango_cmd->buttons |= IN_FORWARD;
			mango_cmd->buttons &= ~IN_BACK;
		}

		if (mango_cmd->sidemove < 0)
		{
			mango_cmd->buttons |= IN_MOVERIGHT;
			mango_cmd->buttons &= ~IN_MOVELEFT;
		}

		if (mango_cmd->sidemove > 0)
		{
			mango_cmd->buttons |= IN_MOVELEFT;
			mango_cmd->buttons &= ~IN_MOVERIGHT;
		}
	}

	void on_create_move(ap::sdk::c_user_cmd* mango_cmd) {
		slide_walk(mango_cmd);
	}
}