#pragma once

#include "../misc/vector.h"


// TODO: funnel all specific stuff here so that its to make portable
namespace toenail
{
	bool is_point_in_bounds(ap::vec2i point, ap::vec2i min, ap::vec2i max);
	float get_timestamp();
	ap::vec2i get_screen_size();
} // namespace toenail