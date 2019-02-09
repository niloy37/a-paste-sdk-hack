#pragma once
#include "../misc/vmt.h"

namespace ap::sdk
{
	class c_client_mode
	{
	public:
		int render_fog()
		{
			using fn = int(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 17)(this);
		}
	private:
	};
} // namespace ap::sdk