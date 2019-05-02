#include "aimbot.h"
#include <algorithm>

#include "../ap_core.h"
#include "../autowall.h"			
#include "../misc/interfaces.h"
#include "../misc/vector.h"
#include "../misc/utils.h"
#include "../misc/defines.h"
#include "../variables.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_base_entity.h"
#include "../sdk/c_base_weapon.h"
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_trace.h"
#include "../sdk/c_client_class.h"
#include "../sdk/c_physics_props.h"
#include "../sdk/surface_data.h"
#include "../misc/defines.h"
#include "../sdk/c_client_entity_list.h"
#include "../misc/globalvars.h"
namespace ap::features::aimbot {
	bool get_hitbox_pos(ap::sdk::c_base_entity* entity, int hitbox_index, ap::vec3f& position)
	{
		const auto hitbox = entity->get_hitbox(hitbox_index);
		if (!hitbox)
			return false;

		const auto bone_matrix = entity->get_bone_matrix(hitbox->bone);

		ap::vec3f bbmin, bbmax;
		ap::vec3f_transform(hitbox->bbmin, bone_matrix, bbmin);
		ap::vec3f_transform(hitbox->bbmax, bone_matrix, bbmax);

		position = (bbmin + bbmax) * 0.5f;
		return true;
	}
	void ragebot::run(ap::sdk::c_user_cmd* cmd) {
		if (!variables::ragebot_test)
			return;

		no_recoil(cmd);
		begin(cmd);
	}

	void ragebot::no_recoil(ap::sdk::c_user_cmd* cmd) {
		if (cmd->buttons & IN_ATTACK)
		{
			vec3f recoil = ap::g::mango_local->get_aim_punch();
			cmd->viewangles -= recoil * 2.f;
		}
	}

	void ragebot::begin(ap::sdk::c_user_cmd* cmd) {
		float best_fov = 180;
		vec3f view_angles, engine_angles, angles, smooth_angle;

		for (int i = 1; i <= 64; ++i)
		{
			auto e = ap::interfaces::client_entity_list->get_client_entity(i);

			//Sanity check
			if (!e || e->IsDormant() || !e->is_alive() || e == g::mango_local || e->get_team_num() == g::mango_local->get_team_num())
				continue;

			//Viewangles
			ap::interfaces::engine->get_viewangles(engine_angles);

			//Getting the bone to aim at
			angles = ap::normalize_angle(ap::calc_angle(ap::g::mango_local->get_eye_position(), ap::get_hitbox_position(e, HITBOX_HEAD)));


			float fov = (vec_length<float, 3, 2>((engine_angles - angles)));

			if (fov < best_fov)
			{
				best_fov = fov;
				view_angles = angles;
			}
		}

		//Making sure they are in view
		if (best_fov != 180)
		{
			if (variables::silent_aim)
				cmd->viewangles = view_angles;
			else
				ap::interfaces::engine->set_viewangle(view_angles);
		}
	}

	ap::sdk::mstudiobbox_t* GetHitbox(ap::sdk::c_base_entity* entity, int hitbox_index)
	{
		if (entity->IsDormant() || entity->get_health() <= 0)
			return NULL;

		const auto pModel = entity->GetModel();
		if (!pModel)
			return NULL;

		auto pStudioHdr = ap::interfaces::model_info->get_studio_hdr(pModel);
		if (!pStudioHdr)
			return NULL;

		auto pSet = pStudioHdr->GetHitboxSet(0);
		if (!pSet)
			return NULL;

		if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
			return NULL;

		return pSet->GetHitbox(hitbox_index);
	}
}