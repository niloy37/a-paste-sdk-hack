#pragma once
#include "toenail.h"
#include "defines.h"
#include "input.h"
#include "../ap_core.h"
#include "../misc/renderer.h"
#include "../misc/interfaces.h"
#include "../sdk/c_surface.h"

namespace ap::menu
{
	void run();

	inline bool menu_open = false;
}