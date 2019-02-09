#pragma once

#include "../misc/vmt.h"


namespace ap::sdk
{
	class c_client_class;

	class c_client
	{
	public:
		c_client_class* get_all_classes()
		{
			using fn = c_client_class* (__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 8)(this);
		}
	};
} // namespace ap::sdk