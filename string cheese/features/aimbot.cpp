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

namespace ap::features::aimbot {
	void aimbot(sdk::c_user_cmd* mango_cmd)
	{
		if (!ap::text_menu::menu::get()._get(L"aim_ragebot_test"))
			return;
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;
		for (int rib = 1; rib <= 64; ++rib)
		{
			ap::sdk::c_base_entity* pPlayerEntity = ap::interfaces::client_entity_list->get_client_entity(rib);
			vec3f Angle = ap::calc_angle(mango_local->get_eye_position(), pPlayerEntity->get_vec_origin());

			if (ap::text_menu::menu::get()._get(L"aim_ragebot_test"))
				mango_cmd->viewangles = ap::normalize_angle(Angle) /*: ap::interfaces::engine->get_viewangles(ap::normalize_angle(Angle))*/;
		}
	}
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
	void on_create_move(sdk::c_user_cmd* mango_cmd) {
		no_recoil();
		//aimbot(mango_cmd);
	}
}

