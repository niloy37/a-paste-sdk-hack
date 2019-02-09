#pragma once
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"

namespace ap::features::fakelag {

	void aimware_adaptive_fakelag()
	//{
	//	ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
	//	if (mango_local == nullptr)
	//		return;
	//
	//	// adaptive
	//	int wish_ticks = 0;
	//	int adaptive_ticks = 2;
	//	mango_local->get_velocity() extrapolated_speed = fsqrt(
	//		(*(*entity + mango_local->get_velocity()) * *(*entity + m_vecVelocity))
	//		+ (*(*entity + m_vecVelocity + 4) * *(*entity + m_vecVelocity + 4)))
	//		* *(g_pGlobals + 0x20);
	//	while ((wish_ticks * extrapolated_speed) <= float_68_0)
	//	{
	//		if (((adaptive_ticks - 1) * extrapolated_speed) > float_68_0)
	//		{
	//			++wish_ticks;
	//			break;
	//		}
	//		if ((adaptive_ticks * extrapolated_speed) > float_68_0)
	//		{
	//			wish_ticks += 2;
	//			break;
	//		}
	//		if (((adaptive_ticks + 1) * extrapolated_speed) > float_68_0)
	//		{
	//			wish_ticks += 3;
	//			break;
	//		}
	//		if (((adaptive_ticks + 2) * extrapolated_speed) > float_68_0)
	//		{
	//			wish_ticks += 4;
	//			break;
	//		}
	//		adaptive_ticks += 5;
	//		wish_ticks += 5;
	//		if (adaptive_ticks > 16)
	//			break;
	//	}
	//	if (*(entity + 0x80) < wish_ticks)
	//		goto LABEL_63;
	//	
	//}
	
}