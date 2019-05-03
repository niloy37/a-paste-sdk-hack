#include "autowall.h"

#include <algorithm>

#include "../string cheese/ap_core.h"

#include "../string cheese/misc/interfaces.h"
#include "../string cheese/misc/vector.h"
#include "../string cheese/misc/utils.h"
#include "../string cheese/misc/defines.h"

#include "../string cheese/sdk/c_engine.h"
#include "../string cheese/sdk/c_client_entity_list.h"
#include "../string cheese/sdk/c_base_entity.h"
#include "../string cheese/sdk/c_base_weapon.h"
#include "../string cheese/sdk/c_user_cmd.h"
#include "../string cheese/sdk/c_trace.h"
#include "../string cheese/sdk/c_client_class.h"
#include "../string cheese/sdk/c_physics_props.h"
#include "../string cheese/sdk/surface_data.h"


namespace
{
	void UTIL_TraceLine(ap::vec3f& absStart, ap::vec3f& absEnd, unsigned int mask, ap::sdk::c_base_entity* ignore, ap::sdk::trace_t* ptr)
	{
		ap::sdk::Ray_t ray;
		ray.Init(absStart, absEnd);
		ap::sdk::CTraceFilter filter;
		filter.pSkip1 = ignore;

		ap::interfaces::trace->trace_ray(ray, mask, &filter, ptr);
	}
	void UTIL_ClipTraceToPlayers(const ap::vec3f& vecAbsStart, const ap::vec3f& vecAbsEnd, unsigned int mask, ap::sdk::ITraceFilter* filter, ap::sdk::trace_t* tr)
	{
		static volatile const auto dwAddress = ap::offsets::get_signature("utilClipTraceToPlayers");
		if (!dwAddress)
			return;

		_asm
		{
			MOV		EAX, filter
			LEA		ECX, tr
			PUSH	ECX
			PUSH	EAX
			PUSH	mask
			LEA		EDX, vecAbsEnd
			LEA		ECX, vecAbsStart
			CALL	dwAddress
			ADD		ESP, 0xC
		}
	}

	void ScaleDamage(ap::sdk::c_base_entity* entity, ap::sdk::c_weapon_info* weapon_info, int hitgroup, float& current_damage)
	{
		bool hasHeavyArmor = false;
		int armorValue = entity->get_armor();

		auto IsArmored = [&entity, &hitgroup]()->bool
		{
			ap::sdk::c_base_entity* targetEntity = entity;
			switch (hitgroup)
			{
			case HITGROUP_HEAD:
				return targetEntity->has_helmet();
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
				return true;
			default:
				return false;
			}
		};

		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			current_damage *= hasHeavyArmor ? 2.f : 4.f; //Heavy Armor does 1/2 damage
			break;
		case HITGROUP_STOMACH:
			current_damage *= 1.25f;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			current_damage *= 0.75f;
			break;
		default:
			break;
		}

		if (armorValue > 0 && IsArmored())
		{
			float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weapon_info->m_ArmorRatio / 2.f;

			//Damage gets modified for heavy armor users
			if (hasHeavyArmor)
			{
				armorBonusRatio = 0.33f;
				armorRatio *= 0.5f;
				bonusValue = 0.33f;
			}

			auto NewDamage = current_damage * armorRatio;

			if (hasHeavyArmor)
				NewDamage *= 0.85f;

			if (((current_damage - (current_damage * armorRatio)) * (bonusValue * armorBonusRatio)) > armorValue)
				NewDamage = current_damage - (armorValue / armorBonusRatio);

			current_damage = NewDamage;
		}
	}
	bool IsBreakableEntity(ap::sdk::c_base_entity * ent)
	{
		typedef bool(__thiscall * isBreakbaleEntityFn)(ap::sdk::c_base_entity*);
		static isBreakbaleEntityFn IsBreakableEntityFn = (isBreakbaleEntityFn)ap::offsets::get_signature("isBreakableEntity");

		if (IsBreakableEntityFn)
		{
			// 0x280 = m_takedamage

			auto backupval = *reinterpret_cast<int*>((uint32_t)ent + 0x280);
			auto className = ent->get_client_class()->m_pNetworkName;

			if (ent != ap::interfaces::client_entity_list->get_client_entity(0))
			{
				// CFuncBrush:
				// (className[1] != 'F' || className[4] != 'c' || className[5] != 'B' || className[9] != 'h')
				if ((className[1] == 'B' && className[9] == 'e' && className[10] == 'S' && className[16] == 'e') // CBreakableSurface
					|| (className[1] != 'B' || className[5] != 'D')) // CBaseDoor because fuck doors
				{
					*reinterpret_cast<int*>((uint32_t)ent + 0x280) = 2;
				}
			}

			bool retn = IsBreakableEntityFn(ent);

			*reinterpret_cast<int*>((uint32_t)ent + 0x280) = backupval;

			return retn;
		}
		else
			return false;
	}
	bool TraceToExit(ap::vec3f & end, ap::sdk::trace_t * enter_trace, ap::vec3f start, ap::vec3f dir, ap::sdk::trace_t * exit_trace)
	{
		float distance = 0.f;
		int first_contents = 0;

		while (distance < 90.f)
		{
			distance += 4.f;
			end = start + (dir * distance);

			if (!first_contents)
				first_contents = ap::interfaces::trace->get_point_contents(end, MASK_SHOT_HULL | CONTENTS_HITBOX);

			const auto point_contents = ap::interfaces::trace->get_point_contents(end, MASK_SHOT_HULL | CONTENTS_HITBOX);
			if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX) || first_contents == point_contents))
				continue;

			auto new_end = end - (dir * 4.f);
			UTIL_TraceLine(end, new_end, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);

			if (exit_trace->startSolid && (exit_trace->surface.flags & SURF_HITBOX) < 0)
			{
				UTIL_TraceLine(end, start, MASK_SHOT_HULL, exit_trace->m_pEnt, exit_trace);

				if (exit_trace->DidHit() && !exit_trace->startSolid)
				{
					end = exit_trace->end;
					return true;
				}
				continue;
			}

			if (!exit_trace->DidHit() || exit_trace->startSolid)
			{
				if (enter_trace->m_pEnt)
				{
					if (enter_trace->DidHitNonWorldEntity() && IsBreakableEntity(enter_trace->m_pEnt))
					{
						*exit_trace = *enter_trace;
						exit_trace->end = start + dir;
						return true;
					}
				}
				continue;
			}

			if ((exit_trace->surface.flags >> 7) & SURF_LIGHT)
			{
				if (IsBreakableEntity(exit_trace->m_pEnt) && IsBreakableEntity(enter_trace->m_pEnt))
				{
					end = exit_trace->end;
					return true;
				}

				if (!((enter_trace->surface.flags >> 7) & SURF_LIGHT))
					continue;
			}

			if (ap::vec_dot(exit_trace->plane.normal, dir) <= 1.f)
			{
				const float fraction = exit_trace->flFraction * 4.f;
				end = end - (dir * fraction);

				return true;
			}
		}
		return false;
	}
	bool HandleBulletPenetration(ap::sdk::c_weapon_info * wpn_data, ap::sdk::trace_t & enter_trace, ap::vec3f direction, ap::vec3f & src, int& penetrate_count, float& current_damage)
	{
		if (penetrate_count <= 0 || wpn_data->m_Penetration <= 0.f)
			return false;

		const bool is_solid_surf = (enter_trace.contents >> 3) & CONTENTS_SOLID;
		const bool is_light_surf = (enter_trace.surface.flags >> 7) & SURF_LIGHT;

		const auto enter_surface_data = ap::interfaces::physics_props->get_surface_data(enter_trace.surface.surfaceProps);
		const auto enter_material = enter_surface_data->game.material;
		const float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

		ap::vec3f dummy;
		ap::sdk::trace_t trace_exit;
		if (!TraceToExit(dummy, &enter_trace, enter_trace.end, direction, &trace_exit))
		{
			if (!(ap::interfaces::trace->get_point_contents(dummy, MASK_SHOT_HULL) & MASK_SHOT_HULL))
				return false;
		}

		const auto exit_surface_data = ap::interfaces::physics_props->get_surface_data(trace_exit.surface.surfaceProps);
		const auto exit_material = exit_surface_data->game.material;

		float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
		float exit_surf_damage_mod = exit_surface_data->game.flDamageModifier;

		float final_damage_modifier = 0.16f;
		float combined_penetration_modifier = 0.f;

		if (enter_material == 89 || enter_material == 71)
		{
			combined_penetration_modifier = 3.f;
			final_damage_modifier = 0.05f;
		}
		else if (is_solid_surf || is_light_surf)
		{
			combined_penetration_modifier = 1.f;
			final_damage_modifier = 0.16f;
		}
		else
			combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;

		if (enter_material == exit_material)
		{
			if (exit_material == 87 || exit_material == 85)
				combined_penetration_modifier = 3.f;
			else if (exit_material == 76)
				combined_penetration_modifier = 2.f;
		}

		const float modifier = std::max(0.f, 1.f / combined_penetration_modifier);
		const float length = vec_length(trace_exit.end - enter_trace.end);
		const float thickness = length * length;
		const float taken_damage = (modifier * 3.f * fmaxf(0.f, (3.f / wpn_data->m_Penetration) * 1.25f) + (current_damage * final_damage_modifier)) + ((thickness * modifier) / 24.f);

		const float lost_damage = std::max(0.f, taken_damage);
		if (lost_damage > current_damage)
			return false;

		if (lost_damage > 0.f)
			current_damage -= lost_damage;

		if (current_damage < 1.f)
			return false;

		src = trace_exit.end;
		penetrate_count--;
		return true;
	}
} // namespace

namespace ap::autowall
{
	c_autowall_info calculate_damage(vec3f start, vec3f end, sdk::c_base_entity* from_entity, sdk::c_base_entity* target_entity)
	{
		// direction 
		const auto direction = vec_normalize(end - start);

		// localplayer unless otherwise provided
		if (!from_entity)
			from_entity = interfaces::client_entity_list->get_client_entity(interfaces::engine->get_local_player());
		if (!from_entity)
			return c_autowall_info();

		// setup filters
		auto filter_local = sdk::CTraceFilter();
		filter_local.pSkip1 = from_entity;
		auto filter_player = sdk::CTraceFilterOneEntity();
		filter_player.pEntity = target_entity;

		// setup filter
		sdk::ITraceFilter * filter = nullptr;
		if (target_entity)
			filter = &filter_player;
		else
			filter = &filter_local;

		// weapon
		const auto weapon = reinterpret_cast<sdk::c_base_weapon*>(interfaces::client_entity_list->get_client_entity(from_entity->get_active_weapon()));
		if (!weapon)
			return c_autowall_info();

		const auto weapon_info = weapon->get_weapon_info();
		if (!weapon_info)
			return c_autowall_info();

		const auto client_class = weapon->get_client_class();
		if (!client_class)
			return c_autowall_info();

		const float range = std::min(weapon_info->m_WeaponRange, vec_length(start - end));
		if (range <= 0.f)
			return c_autowall_info();

		c_autowall_info autowall_info;
		end = start + (direction * range);
		auto current_damage = float(weapon_info->m_WeaponDamage);

		sdk::trace_t enter_trace;
		vec3f current_position = start;

		while (current_damage > 0 && autowall_info.pen_count > 0)
		{
			UTIL_TraceLine(current_position, end, MASK_SHOT | CONTENTS_GRATE, from_entity, &enter_trace);
			UTIL_ClipTraceToPlayers(current_position, current_position + direction * 40.f, MASK_SHOT | CONTENTS_GRATE, filter, &enter_trace);

			const float distance_traced = vec_length(enter_trace.end - start);
			current_damage *= std::pow(weapon_info->m_RangeModifier, (distance_traced / 500.f));

			// if reached the end
			if (enter_trace.flFraction == 1.f)
			{
				ScaleDamage(target_entity, weapon_info, 0, current_damage);

				autowall_info.damage = int(current_damage);
				autowall_info.hitgroup = -1;
				autowall_info.end = enter_trace.end;
				autowall_info.fraction = vec_length(start - autowall_info.end) / range;

				if (target_entity)
					autowall_info.hit_entity = target_entity;
				else
					autowall_info.hit_entity = nullptr;

				return autowall_info;
			}

			// hit an entity
			if (const int hitgroup = enter_trace.hitGroup; hitgroup > 0 && hitgroup <= 7 && enter_trace.m_pEnt)
			{
				// checkles gg
				if ((target_entity && enter_trace.m_pEnt != target_entity) ||
					(!enter_trace.m_pEnt->is_enemy(from_entity)))
					return c_autowall_info();

				ScaleDamage(enter_trace.m_pEnt, weapon_info, enter_trace.hitGroup, current_damage);

				// fill the return info
				autowall_info.damage = int(current_damage);
				autowall_info.hitgroup = enter_trace.hitGroup;
				autowall_info.end = enter_trace.end;
				autowall_info.hit_entity = enter_trace.m_pEnt;

				return autowall_info;
			}

			if (!HandleBulletPenetration(weapon_info, enter_trace, direction, current_position, autowall_info.pen_count, current_damage))
				break;
		}

		return c_autowall_info();
	}
} // namespace ap::autowall