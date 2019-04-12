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
#include "aimbot.h"
#include "../misc/globalvars.h"
#include "../sdk/c_trace.h"
#include "../menu.h"
namespace ap::features::aimbot
{
	void no_recoil()
	{
		if (!ap::text_menu::menu::get()._get(L"aim_no_recoil"))
			return;

		ap::sdk::c_convar* recoilscale = ap::interfaces::cvar->find_var("weapon_recoil_scale");
		if (recoilscale) {
			vec3f qPunchAngles = g::mango_local->get_aim_punch();
			vec3f qAimAngles = g::mango_cmd->viewangles;
			qAimAngles -= qPunchAngles * recoilscale->get_float();
			g::mango_cmd->viewangles = qAimAngles;
		}
	}
}

