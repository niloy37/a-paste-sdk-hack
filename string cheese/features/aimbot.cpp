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

	int get_best_pos(ap::sdk::c_base_entity* entity, vec3f& position)
	{
		get_hitbox_pos(entity, HITBOX_HEAD , position);
		auto eyepos = g::mango_local->get_eye_position();
		auto damage = autowall::calculate_damage(eyepos, position, ap::g::mango_local, entity).damage;
		return damage;
	}

	void ragebot::run(ap::sdk::c_user_cmd* cmd) {
		if (!variables::ragebot_test)
			return;

		//no_recoil(cmd);
		begin(cmd);
	}

	//void ragebot::no_recoil(ap::sdk::c_user_cmd* cmd) {
	//	if (cmd->buttons & IN_ATTACK)
	//	{
	//		vec3f recoil = ap::g::mango_local->get_aim_punch();
	//		cmd->viewangles -= recoil * 2.f;
	//	}
	//}

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


			const auto fov = (vec_length<float, 3, 2>((engine_angles - angles)));

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

	bool hitchance(ap::sdk::c_base_entity* entity, vec3f enter)
	{
		auto RandomFloat = [](float min, float max) -> float
		{
			typedef float(*RandomFloat_t)(float, float);
			static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
			return m_RandomFloat(min, max);
		};
		
		const vec3f angle = ap::normalize_angle(ap::calc_angle((g::mango_local->get_vec_origin() + g::mango_local->get_view_offset()), enter));

		vec3f forward, right, up;
		ap::angle_vector(angle, forward, right, up);
		const auto weapon = static_cast<sdk::c_base_weapon*>(interfaces::client_entity_list->get_client_entity(g::mango_local->get_active_weapon()));
		const auto weapon_info = weapon->get_weapon_info();
		auto iHit = 0.f;
		weapon->update_accuracy_penalty();
		for (auto i = 0; i < 256; i++)
		{
			auto RandomA = RandomFloat(0.0f, 1.0f);
			auto RandomB = 1.0f - RandomA * RandomA;

			RandomA = RandomFloat(0.0f, pi_f * 2.0f);
			RandomB *= weapon->get_spread_cone();

			auto SpreadX1 = (cos(RandomA) * RandomB);
			auto SpreadY1 = (sin(RandomA) * RandomB);
		
			auto RandomC = RandomFloat(0.0f, 1.0f);
			auto RandomF = 1.0f - RandomC * RandomC;

			RandomC = RandomFloat(0.0f, pi_f * 2.0f);
			RandomF *= weapon->get_inaccuracy();

			auto SpreadX2 = (cos(RandomC) * RandomF);
			auto SpreadY2 = (sin(RandomC) * RandomF);
			
			auto fSpreadX = SpreadX1 + SpreadX2;
			auto fSpreadY = SpreadY1 + SpreadY2;

			vec3f vSpreadForward;
			vSpreadForward[0] = forward[0] + (fSpreadX * right[0]) + (fSpreadY * up[0]);
			vSpreadForward[1] = forward[1] + (fSpreadX * right[1]) + (fSpreadY * up[1]);
			vSpreadForward[2] = forward[2] + (fSpreadX * right[2]) + (fSpreadY * up[2]);
			normalize_angle(vSpreadForward);

			vec3f qaNewAngle;
			ap::angle_vector(vSpreadForward, qaNewAngle);
			qaNewAngle = ap::normalize_angle(qaNewAngle);

			vec3f vEnd;
			ap::angle_vector(qaNewAngle, vEnd);
			vEnd = ((g::mango_local->get_vec_origin() + g::mango_local->get_view_offset()) + (vEnd * 8192.f));

			sdk::trace_t trace;
			sdk::CTraceFilterOneEntity2 filter;
			filter.pEntity = entity;
			sdk::Ray_t ray;
			ray.Init(g::mango_local->get_vec_origin() + g::mango_local->get_view_offset(), vEnd);

			ap::interfaces::trace->trace_ray(ray, MASK_ALL, &filter, &trace);
			if (trace.m_pEnt == entity)
				iHit++;

			if ((iHit / 256.f) >= (variables::AIMBOT_HITCHANCE / 100.f))
				return true;
		}
		return false;
	}
	float accepted_inaccuracy(sdk::c_base_weapon* weapon)
	{
		auto local_player = interfaces::client_entity_list->get_client_entity(interfaces::engine->get_local_player());
		if (!local_player) return 0;

		if (!weapon) return 0;
		//if (weapon->get_item_definition_index() == sdk::ItemDefinitionIndex::WEAPON_ZEUS) return 0;

		float inaccuracy = weapon->get_inaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001f;
		inaccuracy = 1 / inaccuracy;
		return inaccuracy;
	}
	void run(sdk::c_user_cmd* cmd)
	{
		if (!variables::ragebot_head_only)
			return;

		const auto local_player = interfaces::client_entity_list->get_client_entity(interfaces::engine->get_local_player());
		if (!local_player || local_player->get_health() <= 0)
			return;

		const auto weapon = reinterpret_cast<sdk::c_base_weapon*>(interfaces::client_entity_list->get_client_entity(local_player->get_active_weapon()));
		if (!weapon)
			return;

		const auto weapon_info = weapon->get_weapon_info();
		if (!weapon_info || weapon_info->m_WeaponType == WEAPON_TYPE_GRENADE || weapon_info->m_WeaponType == WEAPON_TYPE_KNIFE)
			return;

		const auto curtime = local_player->get_tick_base() * interfaces::globals->interval_per_tick;
		if (weapon->get_next_primary_attack() > curtime || local_player->get_next_attack() > curtime)
			return;

		vec3f end_position;
		sdk::c_base_entity * target_entity = nullptr;
		const auto recoil = ap::g::mango_local->get_aim_punch();
		auto best_damage = 0;
		for (auto i = 0; i < 64; i++)
		{
			const auto entity = interfaces::client_entity_list->get_client_entity(i);
			if (!entity || entity->is_dormant() || entity->is_immune() || entity->get_health() <= 0 || !local_player->is_enemy(entity))
				continue;

			vec3f position;
			const auto damage = get_best_pos(entity, position);

			if (damage > best_damage)
			{
				target_entity = entity;
				end_position = position;
				best_damage = damage;
			}
		}

		if (!target_entity || best_damage < std::min(variables::AIMBOT_MIN_DAMAGE, target_entity->get_health() + 10))
			return;

		if (local_player->get_flags() & FL_ONGROUND)
		{
			// autoscope
			if (!local_player->is_scoped() && is_sniper(weapon))
			{
				cmd->buttons |= IN_ATTACK2;
				return;
			}
		
			vec3f viewangles;
			interfaces::engine->get_viewangles(viewangles);
			const auto velocity = local_player->get_velocity();
		
			// autostop
			//if (variables::auto_stop)
			//{
				cmd->sidemove = 0.f;
				cmd->forwardmove = (vec_length(velocity) > 20.f ? 450.f : 0.f);
				rotate_movement(cmd, viewangles, ap::vec3f_angle(velocity)[1] + 180.f);
			//}
		
		}
			//if (!hitchance(target_entity, end_position))
			//	return;
			const auto recoil_compensation = (recoil * 2.f);
			//draw_hitboxes(target_entity, rgba8(0, 200, 255), 10.f);
			cmd->viewangles = (ap::vec3f_angle(end_position - local_player->get_eye_position()) - recoil_compensation);
			cmd->buttons |= IN_ATTACK;
	}
		
}
	