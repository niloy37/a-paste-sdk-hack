#pragma once
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"
#include "../misc/variables.h"
#include "../sdk/c_cvar.h"
namespace ap::features::movement {
	float get_delta(float hspeed, float maxspeed, float airaccelerate)
	{
		auto term = (30.0f - (airaccelerate * maxspeed / 66.0f)) / hspeed;

		if (term < 1.0f && term > -1.0f) {
			return acos(term);
		}

		return 0.f;
	}
	void basic_auto_strafer(ap::sdk::c_user_cmd* mango_cmd) {
		if (!ap::settings::legit_auto_strafer)
			return;

		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		static float circle_yaw = 0.f,
			old_yaw = 0.f;

		if (mango_local->get_move_type() == MoveType::MOVETYPE_LADDER)
			return;
		
		if (!(mango_local->get_flags() & FL_ONGROUND))
		{
			float yaw_delta = ap::normalize_yaw(mango_cmd->viewangles[1] - old_yaw);

			circle_yaw = old_yaw = mango_cmd->viewangles[1];

			if (yaw_delta > 0.f)
				mango_cmd->sidemove = -450.f;
			else if (yaw_delta < 0.f)
				mango_cmd->sidemove = 450.f;
		}
	}

	void post_processing()
	{
		if (ap::settings::post_processing)
		{
			ap::sdk::c_convar* post_processing = ap::interfaces::cvar->find_var("mat_postprocess_enable");
			*(float*)((DWORD)& post_processing->fnChangeCallback + 0xC) = NULL;
			post_processing->set_value("0");
		}
		else
		{
			ap::sdk::c_convar* post_processing = ap::interfaces::cvar->find_var("mat_postprocess_enable");
			*(float*)((DWORD)& post_processing->fnChangeCallback + 0xC) = NULL;
			post_processing->set_value("1");
		}
	}

	void auto_jump(ap::sdk::c_user_cmd* mango_cmd) {
		if (!ap::settings::auto_jump)
			return;
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		if (mango_local->get_move_type() == MoveType::MOVETYPE_LADDER)
			return;

		static bool bLastJumped = false;
		static bool bShouldFake = false;

		if (!bLastJumped && bShouldFake) {
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

	void no_stamina_cooldown(ap::sdk::c_user_cmd* mango_cmd)
	{
		if (!ap::settings::no_crouch_cooldown)
			return;
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;
		mango_cmd->buttons |= IN_BULLRUSH;
	}

	void fast_duck(ap::sdk::c_user_cmd* mango_cmd, bool bSendPackets) {
		if (!ap::settings::fast_crouch_loop)
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
				bSendPackets = true;
			}
			else
				mango_cmd->buttons &= ~IN_DUCK;
		}
	}

	void on_create_move(ap::sdk::c_user_cmd* mango_cmd, bool * bSendPackets)
	{
		fast_duck(mango_cmd, bSendPackets);
		auto_jump(mango_cmd);
		basic_auto_strafer(mango_cmd);
		no_stamina_cooldown(mango_cmd);
		post_processing();
	}

}

