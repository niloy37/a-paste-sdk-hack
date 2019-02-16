#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"

namespace ap::features::movement
{
	void fix_move(vec3f old_angles, ap::sdk::c_user_cmd* mango_cmd, float old_forward, float old_side)
	{
		float view_delta, f1, f2;

		if (old_angles[1] < 0.f)
			f1 = 360.0f + old_angles[1];
		else
			f1 = old_angles[1];

		if (mango_cmd->viewangles[1] < 0.0f)
			f2 = 360.0f + mango_cmd->viewangles[1];
		else
			f2 = mango_cmd->viewangles[1];

		if (f2 < f1)
			view_delta = abs(f2 - f1);
		else
			view_delta = 360.0f - abs(f1 - f2);

		view_delta = 360.0f - view_delta;
		
		//mango_cmd->forwardmove = cos(DirectX::XMConvertToRadians(view_delta)) * old_forward + cos(DirectX::XMConvertToRadians(view_delta + 90.f)) * old_side;
		//mango_cmd->sidemove = sin(DirectX::XMConvertToRadians(view_delta)) * old_forward + sin(DirectX::XMConvertToRadians(view_delta + 90.f)) * old_side;
	}

	void basic_auto_strafer(ap::sdk::c_user_cmd* mango_cmd) {

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

	void auto_jump(ap::sdk::c_user_cmd* mango_cmd) {
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
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;
		mango_cmd->buttons |= IN_BULLRUSH;
	}

	void fast_duck(ap::sdk::c_user_cmd* mango_cmd, bool bSendPackets) {
	//	if (mango_cmd->buttons & IN_DUCK)
	//	{
	//		static bool counter = false;
	//		static int counters = 0;
	//		if (counters == 9)
	//		{
	//			counters = 0;
	//			counter = !counter;
	//		}
	//		counters++;
	//		if (counter)
	//		{
	//			mango_cmd->buttons |= IN_DUCK;
	//			bSendPackets = true;
	//		}
	//		else
	//			mango_cmd->buttons &= ~IN_DUCK;
	//	}
	}

	void on_create_move(ap::sdk::c_user_cmd* mango_cmd, bool * bSendPackets)
	{
		fast_duck(mango_cmd, bSendPackets);
		auto_jump(mango_cmd);
		basic_auto_strafer(mango_cmd);
		no_stamina_cooldown(mango_cmd);
	}

}

