#pragma once
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"
#include "../menu.h"
#include "../sdk/c_cvar.h"
#include "../misc/math.h"

namespace ap::features::movement
{
	float get_delta(float hspeed, float maxspeed, float airaccelerate)
	{
		auto term = (30.0f - (airaccelerate * maxspeed / 66.0f)) / hspeed;

		if (term < 1.0f && term > -1.0f)
		{
			return acos(term);
		}

		return 0.f;
	}

	void basic_auto_strafer(sdk::c_user_cmd* mango_cmd)
	{
		if (!text_menu::menu::get()._get(L"misc_legit_auto_strafer"))
			return;

		auto mango_local = interfaces::client_entity_list->get_client_entity(
			interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		static float circle_yaw = 0.f,
		             old_yaw = 0.f;

		if (mango_local->get_move_type() == MOVETYPE_LADDER)
			return;

		if (!(mango_local->get_flags() & FL_ONGROUND))
		{
			float yaw_delta = normalize_yaw(mango_cmd->viewangles[1] - old_yaw);

			circle_yaw = old_yaw = mango_cmd->viewangles[1];

			if (yaw_delta > 0.f)
				mango_cmd->sidemove = -450.f;
			else if (yaw_delta < 0.f)
				mango_cmd->sidemove = 450.f;
		}
	}

	void post_processing()
	{
		if (variables::post_processing)
		{
			sdk::c_convar* post_processing = interfaces::cvar->find_var("mat_postprocess_enable");
			*(float*)((DWORD)& post_processing->fnChangeCallback + 0xC) = NULL;
			post_processing->set_value("0");
		}
		else
		{
			sdk::c_convar* post_processing = interfaces::cvar->find_var("mat_postprocess_enable");
			*(float*)((DWORD)& post_processing->fnChangeCallback + 0xC) = NULL;
			post_processing->set_value("1");
		}
	}

	void auto_jump(sdk::c_user_cmd* mango_cmd)
	{
		if (!variables::auto_jump)
			return;
		sdk::c_base_entity* mango_local = interfaces::client_entity_list->get_client_entity(
			interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		if (mango_local->get_move_type() == MOVETYPE_LADDER)
			return;

		static bool bLastJumped = false;
		static bool bShouldFake = false;

		if (!bLastJumped && bShouldFake)
		{
			bShouldFake = false;
			mango_cmd->buttons |= IN_JUMP;
		}
		else if (mango_cmd->buttons & IN_JUMP)
		{
			if (mango_local->get_flags() & FL_ONGROUND)
				bShouldFake = bLastJumped = true;
			else
			{
				mango_cmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			}
		}
		else
			bShouldFake = bLastJumped = false;
	}

	void no_stamina_cooldown(sdk::c_user_cmd* mango_cmd)
	{
		if (!variables::no_crouch_cooldown)
			return;
		sdk::c_base_entity* mango_local = interfaces::client_entity_list->get_client_entity(
			interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;
		mango_cmd->buttons |= IN_BULLRUSH;
	}

	void slow_walk(sdk::c_user_cmd* mango_cmd)
	{
		if (variables::slow_walk && toenail::g_input.get_key_state(VK_SHIFT) == toenail::
			keystate::held)
		{
			mango_cmd->sidemove *= 0.1f;
			mango_cmd->forwardmove *= 0.1f;
		}
	}

	void fast_duck(sdk::c_user_cmd* mango_cmd, bool bSendPacket)
	{
		if (!variables::fast_crouch_loop)
			return;
		if (mango_cmd->buttons & IN_DUCK)
		{
			static bool counter = false;
			static int counters = 0;
			if (counters == 9)
			{
				counters = 0;
				counter = !counter;
			}
			counters++;
			if (counter)
			{
				mango_cmd->buttons |= IN_DUCK;
				bSendPacket = true;
			}
			else
				mango_cmd->buttons &= ~IN_DUCK;
		}
	}

	void fix_movement(sdk::c_user_cmd* cmd, vec3f wish_angle)
	{
		vec3f view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
		vec3f viewangles = cmd->viewangles;

		angle_vectors(wish_angle, view_fwd, view_right, view_up);
		angle_vectors(viewangles, cmd_fwd, cmd_right, cmd_up);

		const float v8 = std::sqrtf(view_fwd[0] * view_fwd[0] + view_fwd[1] * view_fwd[1]);
		const float v10 = std::sqrtf(view_right[0] * view_right[0] + view_right[1] * view_right[1]);
		const float v12 = std::sqrtf(view_up[2] * view_up[2]);

		const vec3f norm_view_fwd(1.f / v8 * view_fwd[0], 1.f / v8 * view_fwd[1], 0.f);
		const vec3f norm_view_right(1.f / v10 * view_right[0], 1.f / v10 * view_right[1], 0.f);
		const vec3f norm_view_up(0.f, 0.f, 1.f / v12 * view_up[2]);

		const float v14 = std::sqrtf(cmd_fwd[0] * cmd_fwd[0] + cmd_fwd[1] * cmd_fwd[1]);
		const float v16 = std::sqrtf(cmd_right[0] * cmd_right[0] + cmd_right[1] * cmd_right[1]);
		const float v18 = std::sqrtf(cmd_up[2] * cmd_up[2]);

		const vec3f norm_cmd_fwd(1.f / v14 * cmd_fwd[0], 1.f / v14 * cmd_fwd[1], 0.f);
		const vec3f norm_cmd_right(1.f / v16 * cmd_right[0], 1.f / v16 * cmd_right[1], 0.f);
		const vec3f norm_cmd_up(0.f, 0.f, 1.f / v18 * cmd_up[2]);

		const float v22 = norm_view_fwd[0] * cmd->forwardmove;
		const float v26 = norm_view_fwd[1] * cmd->forwardmove;
		const float v28 = norm_view_fwd[2] * cmd->forwardmove;
		const float v24 = norm_view_right[0] * cmd->sidemove;
		const float v23 = norm_view_right[1] * cmd->sidemove;
		const float v25 = norm_view_right[2] * cmd->sidemove;
		const float v30 = norm_view_up[0] * cmd->upmove;
		const float v27 = norm_view_up[2] * cmd->upmove;
		const float v29 = norm_view_up[1] * cmd->upmove;

		cmd->forwardmove = norm_cmd_fwd[0] * v24 + norm_cmd_fwd[1] * v23 + norm_cmd_fwd[2] * v25
			+ (norm_cmd_fwd[0] * v22 + norm_cmd_fwd[1] * v26 + norm_cmd_fwd[2] * v28)
			+ (norm_cmd_fwd[1] * v30 + norm_cmd_fwd[0] * v29 + norm_cmd_fwd[2] * v27);

		cmd->sidemove = norm_cmd_right[0] * v24 + norm_cmd_right[1] * v23 + norm_cmd_right[2] * v25
			+ (norm_cmd_right[0] * v22 + norm_cmd_right[1] * v26 + norm_cmd_right[2] * v28)
			+ (norm_cmd_right[0] * v29 + norm_cmd_right[1] * v30 + norm_cmd_right[2] * v27);

		cmd->upmove = norm_cmd_up[0] * v23 + norm_cmd_up[1] * v24 + norm_cmd_up[2] * v25
			+ (norm_cmd_up[0] * v26 + norm_cmd_up[1] * v22 + norm_cmd_up[2] * v28)
			+ (norm_cmd_up[0] * v30 + norm_cmd_up[1] * v29 + norm_cmd_up[2] * v27);

		cmd->forwardmove = clamp2(cmd->forwardmove, -450.f, 450.f);
		cmd->sidemove = clamp2(cmd->sidemove, -450.f, 450.f);
		cmd->upmove = clamp2(cmd->upmove, -320.f, 320.f);
	}

	void on_create_move(sdk::c_user_cmd* mango_cmd, bool bSendPackets)
	{
		fast_duck(mango_cmd, bSendPackets);
		auto_jump(mango_cmd);
		basic_auto_strafer(mango_cmd);
		no_stamina_cooldown(mango_cmd);
		slow_walk(mango_cmd);
		post_processing();
	}
}
