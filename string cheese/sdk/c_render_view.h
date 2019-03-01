#pragma once

#include "../misc/vmt.h"

namespace ap::sdk
{
	class c_render_view
	{
	public:
		void SetBlend(float alpha) {
			typedef void(__thiscall* draw_model_execute_fn)(PVOID, float);
			return vmt::get_vfunc<draw_model_execute_fn>(this, 4)(this, alpha);
		}
	};
} // namespace ap::sdk