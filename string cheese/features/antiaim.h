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
#include "../misc/renderer.h"
#include "../sdk/c_cvar.h"
#include "../sdk/c_trace.h"
#include "../misc/globalvars.h"
#include "../menu.h"
#include "../../string cheese/variables.h"
namespace ap::features::antiaim {
	struct angles {
		vec3f Angles;
		vec3f engineViewAngles;
	} _angles;
	void slide_walk(ap::sdk::c_user_cmd* mango_cmd) {
		if (!variables::slide_walk)
			return;
		if (mango_cmd->forwardmove > 0) {
			mango_cmd->buttons |= IN_BACK;
			mango_cmd->buttons &= ~IN_FORWARD;
		}

		if (mango_cmd->forwardmove < 0) {
			mango_cmd->buttons |= IN_FORWARD;
			mango_cmd->buttons &= ~IN_BACK;
		}

		if (mango_cmd->sidemove < 0) {
			mango_cmd->buttons |= IN_MOVERIGHT;
			mango_cmd->buttons &= ~IN_MOVELEFT;
		}

		if (mango_cmd->sidemove > 0) {
			mango_cmd->buttons |= IN_MOVELEFT;
			mango_cmd->buttons &= ~IN_MOVERIGHT;
		}
	}
	//max_speed = fmaxf(animstate->m_active_weapon->get_max_speed(), 0.001f);
	//
	//animstate->m_speed_fraction.x = (1.923077 / max_speed) * animstate->m_speed;
	//animstate->m_speed_fraction.y = (2.9411764 / max_speed) * animstate->m_speed;
	//
	//magic_frac = ((animstate->m_fullspeed_fraction * -0.3f) - 0.2f) * animstate->m_speed_fraction.x;
	//if (animstate->m_duckamount > 0.f)
	//	magic_frac += (animstate->m_speed_fraction.y * animstate->m_duckamount) * (0.5f - magic_frac);
	//
	//animstate->m_old_absyaw = animstate->m_absyaw;
	//animstate->m_absyaw = std::clamp(animstate->m_absyaw, -360.f, 360.f);
	//
	//yaw_delta = eye_yaw - animstate->m_absyaw;
	//normalize(yaw_delta);
	//
	//max_rotation = magic_frac * animstate->m_maxrotation;
	//inv_max_rotation = magic_frac * animstate->m_inv_maxrotation;
	//
	//if (yaw_delta <= max_rotation) {
	//	if (inv_max_rotation > yaw_delta)
	//		animstate->m_absyaw = std::fabs(inv_max_rotation) + eye_yaw;
	//}
	//else
	//	animstate->m_absyaw = eye_yaw - max_rotation;
	//
	//normalize(animstate->m_absyaw);
	//
	//if (animstate->m_speed <= 0.1)
	//	animstate->m_absyaw = ApproachAngle(entity->m_lby(), animstate->m_absyaw, animstate->m_frametime * 100.f);
	//else
	//	animstate->m_absyaw = ApproachAngle(eye_yaw, animstate->m_absyaw, ((animstate->m_acceleration_fraction * 20.f) + 30.f) * animstate->m_frametime);
	float best_head_angle(float yaw) {

		float Back, Right, Left;
		auto mango_local = ap::g::mango_local;

		vec3f src3D, dst3D, forward, right, up, src, dst;
		sdk::trace_t tr;
		sdk::Ray_t ray, ray2, ray3, ray4, ray5;
		sdk::CTraceFilter filter;


		ap::interfaces::engine->get_viewangles(_angles.engineViewAngles);

		_angles.engineViewAngles[0] = 0;

		ap::angle_vectors(_angles.engineViewAngles, forward, right, up);

		filter.pSkip1 = mango_local;
		src3D = mango_local->get_eye_position();
		dst3D = src3D + (forward * 384);

		ray.Init(src3D, dst3D);

		ap::interfaces::trace->trace_ray(ray, MASK_SHOT, &filter, &tr);

		Back = ap::vec_length(tr.end - tr.start);

		ray2.Init(src3D + right * 35, dst3D + right * 35);

		ap::interfaces::trace->trace_ray(ray2, MASK_SHOT, &filter, &tr);

		Right = ap::vec_length(tr.end - tr.start);

		ray3.Init(src3D - right * 35, dst3D - right * 35);

		ap::interfaces::trace->trace_ray(ray3, MASK_SHOT, &filter, &tr);

		Left = ap::vec_length(tr.end - tr.start);

		if (Left > Right)
		{
			return (yaw - 90);
		}
		else if (Right > Left)
		{
			return (yaw + 90);
		}
		else if (Back > Right || Back > Left)
		{
			return (yaw - 180);
		}
		return 0;

	}

	void emotion_pitch_oh_yeah_yeah(ap::sdk::c_user_cmd * mango_cmd) {
		if (!variables::emotion_pitch)
			return;
		mango_cmd->viewangles[0] = 89.f;
	}

	void free_stand(ap::sdk::c_user_cmd * mango_cmd) {
		if (!variables::desync_freestanding)
			return;
		auto mango_local = ap::g::mango_local;

		static float last_real;
		bool no_active = true;
		float bestrotation = 0.f;
		float highestthickness = 0.f;
		vec3f besthead;

		ap::interfaces::engine->get_viewangles(_angles.Angles);

		if (ap::g::b_send_packet) {
			mango_cmd->viewangles[1] = best_head_angle(_angles.Angles[1]) + mango_local->get_max_desync_delta();
			mango_cmd->viewangles[1] = best_head_angle(_angles.Angles[1]) - mango_local->get_max_desync_delta();
		}
		else {
			mango_cmd->viewangles[1] = best_head_angle(_angles.Angles[1]);
		}
		
	}

	void desync(ap::sdk::c_user_cmd* mango_cmd) // fake go ehre
	{
		if (!variables::desync_freestanding)
			return;
		auto mango_local = ap::g::mango_local;
		mango_cmd->viewangles[1] += mango_cmd->command_number % 2 ? -mango_local->get_max_desync_delta() : mango_local->get_max_desync_delta();
		ap::normalize_angle(mango_cmd->viewangles);
	}

	void run_anti_aim_pog(ap::sdk::c_user_cmd * mango_cmd) {
		auto mango_local = ap::g::mango_local;
		if (mango_local->get_health() <= 0) return;
		if (mango_local->get_move_type() == MoveType::MOVETYPE_LADDER) return;
		if (mango_cmd->buttons & IN_USE) return;
		if (mango_cmd->buttons & IN_USE || mango_cmd->buttons & IN_ATTACK || mango_cmd->buttons & IN_ATTACK2 || mango_cmd->buttons & IN_GRENADE1 || mango_cmd->buttons & IN_GRENADE2)
			return;

		if ((vec_length<float, 3, 2>(mango_local->get_velocity())) < 5) // anti lby
		{
			if (mango_cmd->buttons & IN_FORWARD || mango_cmd->buttons & IN_BACK || mango_cmd->buttons & IN_MOVELEFT || mango_cmd->buttons & IN_MOVERIGHT)
				return;
			else
				mango_cmd->forwardmove = mango_cmd->tick_count % 2 ? 1.01f : -1.01f;
		}

		if (ap::g::b_send_packet) {
			desync(mango_cmd);
		}
		else {
			free_stand(mango_cmd);
		}
	}
	void on_create_move(ap::sdk::c_user_cmd* mango_cmd) {
		slide_walk(mango_cmd);
		run_anti_aim_pog(mango_cmd);
	}
}