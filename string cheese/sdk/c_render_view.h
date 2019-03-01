#pragma once

#include "../misc/vmt.h"

namespace ap::sdk
{
	class c_render_view
	{
	public:
		void set_blend(float alpha) {
			typedef void(__thiscall* draw_model_execute_fn)(PVOID, float);
			return vmt::get_vfunc<draw_model_execute_fn>(this, 4)(this, alpha);
		}
		//void set_colour_modulation(rgba8 colour)
		//{
		//	float* blend = { colour.operator[]};
		//	typedef void(__thiscall * draw_model_execute_fn)(PVOID, float*);
		//	vmt::get_vfunc<draw_model_execute_fn>(this, 6)(this, blend);
		//}
		void get_colour_modulation(float* blend) {
			typedef void(__thiscall * draw_model_execute_fn)(PVOID, float*);
			return vmt::get_vfunc<draw_model_execute_fn>(this, 7)(this, blend);
		}
	};
} // namespace ap::sdk