#pragma once
#include "../features/ragebot.h"

namespace ap::features::ragebot 
{
	float get_fov(vec3f qAngles, vec3f vecSource, vec3f vecDestination, bool bDistanceBased)
	{
		auto MakeVector = [](vec3f qAngles)
		{
			auto ret = vec3f();
			auto pitch = float(qAngles[0] * pi / 180.f);
			auto yaw = float(qAngles[1] * pi / 180.f);
			auto tmp = float(cos(pitch));
			ret[0] = float(-tmp * -cos(yaw));
			ret[1] = float(sin(yaw)*tmp);
			ret[2] = float(-sin(pitch));
			return ret;
		};

		vec3f ang, aim;
		double fov;

		ang = ap::calc_angle(vecSource, vecDestination);
		aim = MakeVector(qAngles);
		ang = MakeVector(ang);

		auto mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
		auto mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
		auto u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];

		fov = acos(u_dot_v / (mag_s*mag_d)) * (180.f / pi);

		if (bDistanceBased) {
			fov *= 1.4;
			float xDist = abs(vecSource[0] - vecDestination[0]);
			float yDist = abs(vecSource[1] - vecDestination[1]);
			float Distance = sqrt((xDist * xDist) + (yDist * yDist));

			Distance /= 650.f;

			if (Distance < 0.7f)
				Distance = 0.7f;

			if (Distance > 6.5)
				Distance = 6.5;

			fov *= Distance;
		}

		return (float)fov;
	}


	std::vector<int> get_hitboxes(ap::sdk::c_base_entity* target)
	{
		std::vector<int> hitboxes_toscan;

		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_HEAD);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_BELLY);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_THORAX);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_UPPER_CHEST);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_LOWER_CHEST);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_PELVIS);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_RIGHT_FOOT);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_LEFT_FOOT);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_RIGHT_THIGH);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_LEFT_THIGH);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_RIGHT_CALF);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_LEFT_CALF);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_LEFT_FOREARM);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_RIGHT_FOREARM);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_LEFT_UPPER_ARM);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_RIGHT_UPPER_ARM);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_RIGHT_HAND);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_LEFT_HAND);
		hitboxes_toscan.push_back((int)Hitboxes::HITBOX_NECK);

		return hitboxes_toscan;
	}


	vec3f run_aimscan(ap::sdk::c_base_entity* target, float &simtime, vec3f& origin)
	{
		vec3f vEyePos = g::mango_local->get_eye_position();
		sdk::c_trace* trace;

		for (auto HitboxID : get_hitboxes(target)) {
			int hitgroup = -1;

			if (trace->is_visible(g::mango_local, vEyePos, HitboxID, pTarget, hitgroup)) {
				if (HitboxID != vec3f(0, 0, 0))
					return HitboxID;
			}
		}
		return vec3f(0, 0, 0);
	}

	void drop_target() 
	{
		target_index = -1;
		best_distance = 99999.f;
		aimbotted_in_current_tick = false;
		fired_in_that_tick = false;
		current_aim_position = vec3f();
		pTarget = nullptr;
	}

	void run() 
	{
		if (!ap::settings::enable_ragebot)
			return;

		ap::sdk::c_base_weapon* pWeapon = (ap::sdk::c_base_weapon*)g::mango_local->get_active_weapon();

		if (!pWeapon || pWeapon->get_ammo() == 0)
			return;

		vec3f view; ap::interfaces::engine->get_viewangles(view);

		for (int i = 1; i < 64; ++i) {
			if (i == ap::interfaces::engine->get_local_player())
				continue;

			ap::sdk::c_base_entity* target = ap::interfaces::client_entity_list->get_client_entity(i);

			if (!target->is_valid())
				continue;

			float fov = get_fov(view, g::mango_local->get_eye_position(), target->get_eye_position(), false);

			if (fov > 180.f)
				continue;

			float simtime = 0.f;
			vec3f minus_origin = vec3f(0, 0, 0);
			vec3f aim_position = run_aimscan(target, simtime, minus_origin);

			if (aim_position == vec3f(0, 0, 0))
				continue;

			float selection_value = 0.f;
			selection_value = fov;

			if (best_distance >= selection_value && aim_position != vec3f(0, 0, 0)) {
				best_distance = selection_value;
				target_index = i;
				current_aim_position = aim_position;
				pTarget = target;
				current_aim_simulationtime = simtime;
				current_aim_player_origin = minus_origin;
				// G::Target = target;
			}
		}

		if (target_index != -1 && current_aim_position != vec3f(0, 0, 0) && pTarget) {
			ap::calc_angle(g::mango_local->get_eye_position(), current_aim_position);
			aimbotted_in_current_tick = true;

			if (ap::settings::auto_scope && ap::is_sniper(pWeapon) && !g::mango_local->is_scoped())
				g::mango_cmd->buttons |= IN_ATTACK2;

			if (!ap::settings::silent_aim)
				ap::interfaces::engine->set_viewangle(g::mango_cmd->viewangles);

			if (ap::settings::auto_shoot) {
				//if (ap::settings::hitchance == 0.f || hitchance(g::mango_cmd, pTarget, ap::settings::hitchance)) {
					static bool aPistol = false;
					if (ap::is_pistol(pWeapon)) {
						if (aPistol)
							g::mango_cmd->buttons &= ~IN_ATTACK;
						else
							g::mango_cmd->buttons |= IN_ATTACK;

						fired_in_that_tick = true;
						g::bSendPacket = true;
						aPistol = !aPistol;
					}
					else {
						fired_in_that_tick = true;
						g::bSendPacket = true;
						g::mango_cmd->buttons |= IN_ATTACK;
					}
				//}
			}

			//if (g_pMenu->Ragebot.PositionAdjustment)
			//	G::UserCmd->tick_count = TIME_TO_TICKS(current_aim_simulationtime + g_Backtrack->GetLerpTime());

		}
	}

	void no_recoil()
	{
		if (!ap::settings::enable_ragebot)
			return;

		ap::sdk::c_convar* recoilscale = ap::interfaces::cvar->find_var("weapon_recoil_scale");
		if (recoilscale) {
			vec3f qPunchAngles = g::mango_local->get_aim_punch();
			vec3f qAimAngles = g::mango_cmd->viewangles;
			qAimAngles -= qPunchAngles * recoilscale->get_float();
			g::mango_cmd->viewangles = qAimAngles;
		}
	}

	void on_create_move() {
		drop_target();
		run();
		no_recoil();
	}
}