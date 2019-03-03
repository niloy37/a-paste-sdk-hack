#pragma once

#include "../misc/vmt.h"
#include "../sdk/c_handle.h"

namespace ap::sdk
{
	class c_base_entity;
	struct SpatializationInfo_t;
	class c_client_entity;
	class c_client_entity_list
	{
	public:
		c_base_entity* get_client_entity(int index)
		{
			using fn = c_base_entity* (__thiscall*)(void*, int);
			return vmt::get_vfunc<fn>(this, 3)(this, index);
		}
		int get_highest_index()
		{
			using fn = int (__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 8)(this);
		}
		c_client_entity* get_client_entity_from_handle(c_base_handle hEnt) {
			using fn = c_client_entity * (__thiscall*)(void*, c_base_handle);
			return vmt::get_vfunc<fn>(this, 4)(this, hEnt);
		}
	private:
	};
} // namespace ap::sdk