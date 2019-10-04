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
//#include "../misc/color.h"
namespace ap::features::aimbot {

	int hit_scan(ap::sdk::c_base_entity* entity, vec3f position)
	{
		std::vector<int> HitBoxesToScan;
		int const HitScanMode = variables::rage::get().ragebot_head_only;
		if (HitScanMode)
		{
			HitBoxesToScan.push_back((int)Hitboxes::HITBOX_HEAD);
			HitBoxesToScan.push_back((int)Hitboxes::HITBOX_NECK);
			HitBoxesToScan.push_back((int)Hitboxes::HITBOX_UPPER_CHEST);
			HitBoxesToScan.push_back((int)Hitboxes::HITBOX_UPPER_CHEST);
			HitBoxesToScan.push_back((int)Hitboxes::HITBOX_LOWER_CHEST);
			HitBoxesToScan.push_back((int)Hitboxes::HITBOX_PELVIS);
			HitBoxesToScan.push_back((int)Hitboxes::HITBOX_LEFT_UPPER_ARM);
			HitBoxesToScan.push_back((int)Hitboxes::HITBOX_RIGHT_UPPER_ARM);
			HitBoxesToScan.push_back((int)Hitboxes::HITBOX_LEFT_THIGH);
			HitBoxesToScan.push_back((int)Hitboxes::HITBOX_RIGHT_THIGH);
		}
		return -1;
	}
	
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
		//ap::get_hitbox_position(entity, hit_scan(entity, position));
		get_hitbox_pos(entity, hit_scan(entity, position) , position);
		auto eyepos = g::mango_local->get_eye_position();
		auto damage = autowall::calculate_damage(eyepos, position, ap::g::mango_local, entity).damage;
		return damage;
	}

	

	ap::sdk::mstudiobbox_t* GetHitbox(ap::sdk::c_base_entity* entity, int hitbox_index)
	{
		if (entity->IsDormant() || entity->get_health() <= 0)
			return nullptr;

		const auto pModel = entity->GetModel();
		if (!pModel)
			return nullptr;

		auto pStudioHdr = ap::interfaces::model_info->get_studio_hdr(pModel);
		if (!pStudioHdr)
			return nullptr;

		auto pSet = pStudioHdr->GetHitboxSet(0);
		if (!pSet)
			return nullptr;

		if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
			return nullptr;

		return pSet->GetHitbox(hitbox_index);
	}

	bool hitchance(ap::sdk::c_base_entity* entity, const ap::vec3f& position, float hc)
	{
		const auto local_player = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (!local_player)
			return false;

		const auto weapon = reinterpret_cast<ap::sdk::c_base_weapon*>(ap::interfaces::client_entity_list->get_client_entity(local_player->get_active_weapon()));
		if (!weapon)
			return false;

		const auto random_float = [](float min, float max) -> float
		{
			using fn = float(*)(float, float);
			static const auto func = reinterpret_cast<fn>(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
			return func(min, max);
		};

		const auto eye_position = local_player->get_eye_position();
		const auto angle = ap::normalize_angle(ap::vec3f_angle(position - eye_position));

		ap::vec3f forward, right, up;
		ap::angle_vec3f(angle, forward, right, up);

		int num_hit = 0;
		weapon->update_accuracy_penalty();
		for (int i = 0; i < 256; i++)
		{
			float random_a = random_float(0.0f, 1.0f);
			float random_b = 1.0f - random_a * random_a;

			random_a = random_float(0.0f, ap::pi_f * 2.0f);
			random_b *= weapon->get_spread_cone();

			const float spread_x1 = (cos(random_a) * random_b);
			const float spread_y1 = (sin(random_a) * random_b);

			float random_c = random_float(0.0f, 1.0f);
			float random_f = 1.0f - random_c * random_c;

			random_c = random_float(0.0f, ap::pi_f * 2.0f);
			random_f *= weapon->get_inaccuracy();

			const float spread_x2 = (cos(random_c) * random_f);
			const float spread_y2 = (sin(random_c) * random_f);

			const float spread_x = spread_x1 + spread_x2;
			const float spread_y = spread_y1 + spread_y2;

			ap::vec3f spread_forward;
			spread_forward[0] = forward[0] + (spread_x * right[0]) + (spread_y * up[0]);
			spread_forward[1] = forward[1] + (spread_x * right[1]) + (spread_y * up[1]);
			spread_forward[2] = forward[2] + (spread_x * right[2]) + (spread_y * up[2]);
			spread_forward = ap::vec_normalize(spread_forward);

			ap::vec3f new_angle = ap::vec3f_angle(spread_forward);
			new_angle = ap::normalize_angle(new_angle);

			ap::vec3f end;
			end = ap::angle_vec3f(new_angle);
			end = eye_position + (end * 8192.f);

			ap::sdk::trace_t trace;
			ap::sdk::Ray_t ray;
			ray.Init(eye_position, end);

			ap::sdk::CTraceFilterOneEntity2 filter;
			filter.pEntity = entity;

			ap::interfaces::trace->trace_ray(ray, MASK_ALL, &filter, &trace);
			if (trace.m_pEnt == entity)
				num_hit++;
		}

		return (num_hit / 256.f >= hc * 0.01f);
	}

	

	void run(sdk::c_user_cmd* cmd)
	{
		if (!variables::rage::get().ragebot_head_only)
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

		const auto curtime = (local_player->get_tick_base() * interfaces::globals->interval_per_tick);
		if (weapon->get_next_primary_attack() > curtime || local_player->get_next_attack() > curtime)
			return;

		vec3f end_position;
		sdk::c_base_entity * target_entity = nullptr;
		const auto recoil = local_player->get_aim_punch();
		auto best_damage = 0;
		for (auto i = 0; i < 64; i++)
		{
			const auto entity = interfaces::client_entity_list->get_client_entity(i);
			if (!entity || entity->IsDormant() || entity->is_immune() || entity->get_health() <= 0 || !local_player->is_enemy(entity))
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

		if (!target_entity || best_damage < std::min(variables::rage::get().AIMBOT_MIN_DAMAGE, target_entity->get_health() + 10))
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
			cmd->sidemove = 0.f;
			cmd->forwardmove = (vec_length(velocity) > 20.f ? 450.f : 0.f);
			rotate_movement(cmd, viewangles, ap::vec3f_angle(velocity)[1] + 180.f);

		
		}
		
			if (!hitchance(target_entity, end_position, variables::rage::get().AIMBOT_HITCHANCE))
				return;
		
			const auto recoil_compensation = (recoil * 2.f);
			//draw_hitboxes(target_entity, rgba8(0, 200, 255), 10.f);
			cmd->viewangles = ap::vec3f_angle((end_position - local_player->get_eye_position() ));
		
			cmd->buttons |= IN_ATTACK;
	}
		
}
	