#pragma once

#include "../misc/vmt.h"


namespace ap::sdk
{
	struct surfacedata_t;

	class c_physics_props
	{
	public:
		surfacedata_t *get_surface_data(int surfaceDataIndex)
		{
			using fn = surfacedata_t*(__thiscall*)(void*, int);
			return vmt::get_vfunc<fn>(this, 5)(this, surfaceDataIndex);
		}

	private:
	};
} // namespace ap::sdk