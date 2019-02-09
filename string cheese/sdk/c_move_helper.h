#pragma once

#include "../misc/vmt.h"


namespace ap::sdk
{
	class c_base_entity;

	class c_move_helper
	{
	public:
		virtual void UnknownVirtual() = 0;
	public:

		virtual void SetHost(c_base_entity* host) = 0;
		virtual void ProcessImpacts()
		{
			typedef void(__thiscall* Fn)(void*);
			vmt::get_vfunc<Fn>(this, 4)(this);
		}
	};
} // namespace ap::sdk