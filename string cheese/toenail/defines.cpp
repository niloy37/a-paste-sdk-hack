#include "defines.h"

#include <chrono>

#include "../misc/interfaces.h"
#include "../sdk/c_engine.h"


namespace toenail
{
	bool is_point_in_bounds(ap::vec2i point, ap::vec2i min, ap::vec2i max)
	{
		if (point[0] < min[0] || point[0] > max[0])
			return false;

		if (point[1] < min[1] || point[1] > max[1])
			return false;

		return true;
	}
	float get_timestamp()
	{
		static const auto start = std::chrono::high_resolution_clock::now();
		return float(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count()) / 1000000000.f;
	}
	ap::vec2i get_screen_size()
	{
		ap::vec2i screen_size;
		ap::interfaces::engine->get_screen_size(screen_size);
		return screen_size;
	}
} // namespace toenail