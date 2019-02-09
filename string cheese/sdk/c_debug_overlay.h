#pragma once

#include "../misc/vmt.h"
#include "../misc/vector.h"
#include "../misc/color.h"


namespace ap::sdk
{
	class c_debug_overlay
	{
	public:
		void draw_pill(const vec3f& mins, const vec3f& max, float& diameter, int r, int g, int b, int a, float duration)
		{
			using fn = void(__thiscall*)(void*, const vec3f&, const vec3f&, float&, int, int, int, int, float);
			vmt::get_vfunc<fn>(this, 24)(this, mins, max, diameter, r, g, b, a, duration);
		}

	private:
	};
} // namespace ap::sdk