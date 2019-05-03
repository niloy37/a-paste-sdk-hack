#pragma once

#include "../string cheese/misc/vector.h"


namespace ap::sdk
{
	class c_base_entity;
} // namespace ap::sdk

namespace ap::autowall
{
	struct c_autowall_info
	{
		c_autowall_info()
		{
			damage = -1;
			fraction = 1.f;
			pen_count = 4;
			hit_entity = nullptr;
		}

		int damage,
			hitgroup,
			pen_count;
		float fraction;
		vec3f end;
		sdk::c_base_entity* hit_entity;
	};
	c_autowall_info calculate_damage(vec3f start, vec3f end, sdk::c_base_entity* ignore_entity = nullptr, sdk::c_base_entity* target_entity = nullptr);
} // namespace ap::autowall