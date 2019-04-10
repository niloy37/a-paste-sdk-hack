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

namespace ap::features::antiaim {
	struct angles {
		vec3f Angles;
		vec3f engineViewAngles;
	} _angles;
	void slide_walk(ap::sdk::c_user_cmd* mango_cmd) {
		if (!ap::settings::slide_walk)
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
	float best_head_angle(float yaw) {

		float Back, Right, Left;
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return 0.0f;

		vec3f src3D, dst3D, forward, right, up, src, dst;
		sdk::trace_t tr;
		sdk::Ray_t ray, ray2, ray3, ray4, ray5;
		sdk::CTraceFilter* filter;


		ap::interfaces::engine->get_viewangles(_angles.engineViewAngles);

		_angles.engineViewAngles[0] = 0;

		ap::angle_vector(_angles.engineViewAngles, forward, right, up);

		filter->pSkip1 = mango_local;
		src3D = mango_local->get_eye_position();
		dst3D = src3D + (forward * 384);

		ray.Init(src3D, dst3D);

		ap::interfaces::trace->trace_ray(ray, MASK_SHOT, filter, &tr);

		Back = ap::vec_length(tr.end - tr.start);

		ray2.Init(src3D + right * 35, dst3D + right * 35);

		ap::interfaces::trace->trace_ray(ray2, MASK_SHOT, filter, &tr);

		Right = ap::vec_length(tr.end - tr.start);

		ray3.Init(src3D - right * 35, dst3D - right * 35);

		ap::interfaces::trace->trace_ray(ray3, MASK_SHOT, filter, &tr);

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

		mango_cmd->viewangles[0] = 89.f;
	}

	void desync_free_stand(ap::sdk::c_user_cmd * mango_cmd) {

		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		static float last_real;
		bool no_active = true;
		float bestrotation = 0.f;
		float highestthickness = 0.f;
		vec3f besthead;

		ap::interfaces::engine->get_viewangles(_angles.Angles);

		if (ap::g::b_send_packet)
		{
			mango_cmd->viewangles[1] = best_head_angle(_angles.Angles[1]) + mango_local->get_max_desync_delta();
			mango_cmd->viewangles[1] = best_head_angle(_angles.Angles[1]) - mango_local->get_max_desync_delta();
		}
		else
			mango_cmd->viewangles[1] = best_head_angle(_angles.Angles[1]);
	}

	void run_anti_aim_pog(ap::sdk::c_user_cmd * mango_cmd) {
		//ghetto fix
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;
		if (!ap::settings::antiaim)
			return;
		if (mango_local->get_health() <= 0) return;
		if (mango_local->get_move_type() == MoveType::MOVETYPE_LADDER) return;
		if (mango_cmd->buttons & IN_USE) return;
		if (mango_cmd->buttons & IN_ATTACK)
			return;
		emotion_pitch_oh_yeah_yeah(mango_cmd);
		desync_free_stand(mango_cmd);
	}
	void on_create_move(ap::sdk::c_user_cmd* mango_cmd) {
		slide_walk(mango_cmd);
	}
}