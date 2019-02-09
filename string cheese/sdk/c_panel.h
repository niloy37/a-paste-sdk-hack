#pragma once

#include "../misc/vmt.h"


namespace ap::sdk
{
	class c_panel
	{
	public:
		const char* get_name(unsigned int panel)
		{
			using fn = const char*(__thiscall*)(void*, unsigned int);
			return (vmt::get_vfunc<fn>(this, 36))(this, panel);
		}
		void set_mouse_input_enabled(unsigned int panel, bool state)
		{
			using fn = void(__thiscall *)(PVOID, int, bool);
			return (vmt::get_vfunc<fn>(this, 32))(this, panel, state);
		}

	private:
	};
} // namespace ap::sdk