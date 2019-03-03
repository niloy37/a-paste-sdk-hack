#pragma once
#include "../features/autowall.h"
namespace ap::features::autowall {
	struct fire_bullet_data
	{
		fire_bullet_data(const ap::vec3f& eyePos, ap::sdk::c_base_entity* entity) : src(eyePos), filter(entity)
		{
		}

		vec3f					src;
		ap::sdk::trace_t			 enter_trace;
		vec3f					direction;
		ap::sdk::CTraceFilter   filter;
		float					trace_length;
		float					trace_length_remaining;
		float					current_damage;
		int						penetrate_count;
	};
	void trace_line(ap::vec3f& vecAbsStart, ap::vec3f& vecAbsEnd, unsigned int mask, ap::sdk::c_base_entity* ignore, ap::sdk::trace_t* ptr)
	{
		ap::sdk::CTraceFilter filter(ignore);
		ap::interfaces::trace->trace_ray(ap::sdk::Ray_t(vecAbsStart, vecAbsEnd), mask, &filter, ptr);
	}

	bool vector_to_vector_visible(ap::vec3f src, ap::vec3f point)
	{
		ap::sdk::trace_t trace_init;
		trace_line(src, point, MASK_SOLID, g::mango_local, &trace_init);
		ap::sdk::trace_t Trace;
		trace_line(src, point, MASK_SOLID, trace_init.m_pEnt, &Trace);

		if (Trace.flFraction == 1.0f || trace_init.flFraction == 1.0f)
			return true;

		return false;
	}

	float get_hitgroup_damage_multiple(int iHitGroup)
	{
		switch (iHitGroup)
		{
		case HITGROUP_HEAD:
			return 4.f;
		case HITGROUP_STOMACH:
			return 1.25f;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			return 0.75f;
		}

		return 1.0f;
	}

	bool handle_bullet_penetration(ap::sdk::c_weapon_info * wpn_data, fire_bullet_data & data, bool extracheck, ap::vec3f point);

	void scale_damage(int hitgroup, ap::sdk::c_base_entity * enemy, float weapon_armor_ratio, float& current_damage)
	{
		current_damage *= get_hitgroup_damage_multiple(hitgroup);

		if (enemy->get_armor() > 0.0f && hitgroup < HITGROUP_LEFTLEG)
		{
			if (hitgroup == HITGROUP_HEAD && !enemy->has_helmet())
				return;

			float armorscaled = (weapon_armor_ratio * 0.5f) * current_damage;
			if ((current_damage - armorscaled) * 0.5f > enemy->get_armor())
				armorscaled = current_damage - (enemy->get_armor() * 2.0f);
			current_damage = armorscaled;
		}
	}

	void clip_trace_to_player(vec3f& start, vec3f& end, ap::sdk::c_base_entity* e, unsigned int mask, ap::sdk::ITraceFilter* filter, ap::sdk::trace_t* old_trace) {
		if (!e)
			return;

		vec3f bbmin, bbmax;
		e->GetRenderBounds(bbmin, bbmax);

		ap::vec3f dir(end - start);
		(vec_normalize(dir));

		ap::vec3f
			center = (bbmax + bbmin) / 2,
			pos(center + e->get_vec_origin());

		ap::vec3f to = pos - start;
		float range_along = (vec_dot(dir, to));

		float range;
		if (range_along < 0.f)
			range = -(vec_length(to));

		else if (range_along > (vec_length(dir)))
			range = -(vec_length(pos - end));

		else {
			auto ray(pos - (dir * range_along + start));
			range = (vec_length(ray));
		}

		if (range <= 60.f) {
			ap::sdk::trace_t trace;

			ap::sdk::Ray_t ray;
			ray.Init(start, end);

			ap::interfaces::trace->clip_ray_to_exit(ray, mask, e, &trace);

			if (old_trace->flFraction > trace.flFraction)
				* old_trace = trace;
		}
	}

	bool simulate_fire_bullet(ap::sdk::c_base_entity* local, ap::sdk::c_base_weapon* weapon, fire_bullet_data& data)
	{
		data.penetrate_count = 4;
		data.trace_length = 0.0f;
		auto* wpn_data = weapon->get_weapon_info();
		data.current_damage = (float)wpn_data->m_Damage;
		while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
		{
			data.trace_length_remaining = wpn_data->m_Range - data.trace_length;
			ap::vec3f End_Point = data.src + data.direction * data.trace_length_remaining;
			trace_line(data.src, End_Point, 0x4600400B, local, &data.enter_trace);
			End_Point = End_Point * 40.f;
			auto trace = data.enter_trace;
			clip_trace_to_player(data.src, End_Point, local, 0x4600400B, &data.filter, &trace);
			if (data.enter_trace.flFraction == 1.0f)
				break;
			if ((data.enter_trace.hitGroup <= 7) && (data.enter_trace.hitGroup > 0) && (local->get_team_num() != data.enter_trace.m_pEnt->get_team_num()))
			{
				data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
				data.current_damage *= pow(wpn_data->m_RangeModifier, data.trace_length * 0.002f);
				scale_damage(data.enter_trace.hitGroup, data.enter_trace.m_pEnt, wpn_data->m_ArmorRatio, data.current_damage);
				return true;
			}
			vec3f rommeny;
			bool wobbley = false;
			if (!handle_bullet_penetration(wpn_data, data, wobbley, rommeny))
				break;
		}
		return false;
	}

	bool trace_to_exit_alternative(ap::vec3f& end, ap::sdk::trace_t& tr, ap::vec3f start, ap::vec3f vEnd, ap::sdk::trace_t* trace)
	{
		typedef bool(__fastcall * TraceToExitFn)(ap::vec3f&, ap::sdk::trace_t&, float, float, float, float, float, float, ap::sdk::trace_t*);
		static TraceToExitFn oTraceToExit = (TraceToExitFn)ap::find_signature("client_panorama.dll", "55 8B EC 83 EC 30 F3 0F 10 75");

		if (!oTraceToExit)
			return false;

		return oTraceToExit(end, tr, start[0], start[1], start[2], vEnd[0], vEnd[1], vEnd[2], trace);
	}

	bool handle_bullet_penetration(ap::sdk::c_weapon_info* wpn_data, fire_bullet_data& data, bool extracheck, ap::vec3f point)
	{
		ap::sdk::surfacedata_t* enter_surface_data = ap::interfaces::physics_props->get_surface_data(data.enter_trace.surface.surfaceProps);
		int enter_material = enter_surface_data->game.material;
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
		data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
		data.current_damage *= pow(wpn_data->m_RangeModifier, (data.trace_length * 0.002f));
		if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
			data.penetrate_count = 0;
		if (data.penetrate_count <= 0)
			return false;
		ap::vec3f dummy;
		ap::sdk::trace_t trace_exit;
		if (!trace_to_exit_alternative(dummy, data.enter_trace, data.enter_trace.end, data.direction, &trace_exit))
			return false;
		ap::sdk::surfacedata_t * exit_surface_data = ap::interfaces::physics_props->get_surface_data(trace_exit.surface.surfaceProps);
		int exit_material = exit_surface_data->game.material;
		float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
		float final_damage_modifier = 0.16f;
		float combined_penetration_modifier = 0.0f;
		if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
		{
			combined_penetration_modifier = 3.0f;
			final_damage_modifier = 0.05f;
		}
		else
			combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
		if (enter_material == exit_material)
		{
			if (exit_material == 87 || exit_material == 85)combined_penetration_modifier = 3.0f;
			else if (exit_material == 76)combined_penetration_modifier = 2.0f;
		}
		float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
		float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->m_Penetration) * 1.25f);
		float thickness = ap::vec_length(trace_exit.end - data.enter_trace.end);
		if (extracheck)
			if (!vector_to_vector_visible(trace_exit.end, point))
				return false;
		thickness *= thickness;
		thickness *= v34;
		thickness /= 24.0f;
		float lost_damage = fmaxf(0.0f, v35 + thickness);
		if (lost_damage > data.current_damage)
			return false;
		if (lost_damage >= 0.0f)
			data.current_damage -= lost_damage;
		if (data.current_damage < 1.0f)
			return false;
		data.src = trace_exit.end;
		data.penetrate_count--;

		return true;
	}

	float damage(const ap::vec3f & point)
	{
		auto data = fire_bullet_data(g::mango_local->get_eye_position(), g::mango_local);

		ap::vec3f angles;
		
		angles = ap::calc_angle(data.src, point);
		ap::angle_vector(angles, data.direction);
		ap::vec_normalize(data.direction);
		ap::sdk::c_base_weapon* pWeapon = (ap::sdk::c_base_weapon*)g::mango_local->get_active_weapon();
		if (simulate_fire_bullet(g::mango_local, pWeapon, data))
			return data.current_damage;

		return 0.f;
	}

	bool can_hit_floating_point(const ap::vec3f & point, const ap::vec3f & source) 
	{
		if (!g::mango_local)
			return false;

		fire_bullet_data data = fire_bullet_data(source, g::mango_local);

		ap::vec3f angles = ap::calc_angle(data.src, point);
		ap::angle_vector(angles, data.direction);
		ap::vec_normalize(data.direction);

		ap::sdk::c_base_weapon* pWeapon = (ap::sdk::c_base_weapon*)g::mango_local->get_active_weapon();

		if (!pWeapon)
			return false;

		data.penetrate_count = 1;
		data.trace_length = 0.0f;

		ap::sdk::c_weapon_info* weaponData = pWeapon->get_weapon_info();

		if (!weaponData)
			return false;

		data.current_damage = (float)weaponData->m_Damage;
		data.trace_length_remaining = weaponData->m_Range - data.trace_length;
		ap::vec3f end = data.src + (data.direction * data.trace_length_remaining);
		trace_line(data.src, end, MASK_SHOT | CONTENTS_HITBOX, g::mango_local, &data.enter_trace);
		bool wobbley = true;
		if (vector_to_vector_visible(data.src, point) || handle_bullet_penetration(weaponData, data, wobbley, point))
			return true;

		return false;
	}
}