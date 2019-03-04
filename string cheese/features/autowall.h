#pragma once
#include <list>
#include <algorithm>
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"
#include "../sdk/c_client_state.h"
#include "../sdk/c_globals.h"
#include "../sdk/c_player_info.h"
#include "../features/backtrack.h"
#include "../misc/renderer.h"
#include "../sdk/c_cvar.h"
#include "../misc/variables.h"
#include "../misc/singleton.h"
#include "../sdk/c_trace.h"
#include "../sdk/c_base_weapon.h"
#include "../misc/globalvars.h"
#include "../sdk/surface_data.h"
#include "../sdk/c_physics_props.h"

namespace ap::features::autowall {
	
	bool can_hit_floating_point(const vec3f& point, const vec3f& source);
	float damage(const vec3f& point);
}