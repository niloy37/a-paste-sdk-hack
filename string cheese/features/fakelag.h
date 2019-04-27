#pragma once
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"
#include "../misc/globalvars.h"
#include "../sdk/c_globals.h"
#include "../menu.h"
namespace ap::features::fakelag {

	void fakelag_adaptive(int ticks) // pasted from reversed aimware :^) reversed aimware pasted from chance :^))
	{
		if (!ap::text_menu::menu::get()._get(L"misc_fakelag_adaptive"))
			return;

		int wish_ticks = 0, adaptive_ticks = 2;
		float extrapolated_speed = vec_length<float, 3, 2>(ap::g::mango_local->get_velocity()) * (ap::interfaces::globals->interval_per_tick);
		while ((wish_ticks * extrapolated_speed) <= 68.f) {
			if (((adaptive_ticks - 1) * extrapolated_speed) > 68.f) {
				++wish_ticks;
				break;
			}
			if ((adaptive_ticks * extrapolated_speed) > 68.f) {
				wish_ticks += 2;
				break;
			}
			if (((adaptive_ticks + 1) * extrapolated_speed) > 68.f) {
				wish_ticks += 3;
				break;
			}
			if (((adaptive_ticks + 2) * extrapolated_speed) > 68.f) {
				wish_ticks += 4;
				break;
			}

			adaptive_ticks += 5;
			wish_ticks += 5;

			if (adaptive_ticks > 16)
				break;

			ap::g::b_send_packet = (ticks >= wish_ticks);
		}
	}
	
}