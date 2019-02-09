#pragma once

#include "../misc/vmt.h"


namespace ap::sdk
{
	class c_base_entity;

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

	private:
	};
} // namespace ap::sdk