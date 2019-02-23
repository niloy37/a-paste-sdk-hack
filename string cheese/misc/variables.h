#pragma once
#include <string>

namespace ap::settings {

#define varsetting(type, name, val) type name = val

		varsetting(static bool, ribbit, false);

		/* anti-aim */

		varsetting(static bool, slide_walk, false);

		/* visuals */

		varsetting(static bool, name_esp, true);
		varsetting(static bool, health_esp, true);
		varsetting(static bool, esp_corner_box, true);
		varsetting(static bool, backtrack_stick_figure, true);
		varsetting(static bool, dont_render_team, true);
		varsetting(static bool, armour_flags, true);
		varsetting(static bool, visualize_backtrack_dots, true);
		varsetting(static bool, visualize_backtrack_octagon, false);
		varsetting(static bool, no_scope_lines, true);
		varsetting(static bool, world_modulation, false);
		varsetting(static bool, snap_lines, false);
		varsetting(static bool, remove_smoke, true);
		varsetting(static bool, no_flash, true);
		varsetting(static bool, force_crosshair, true);
		varsetting(static bool, render_custom_crosshair, false);

		/* aim */

		varsetting(static bool, legit_backtrack, true);

		/* misc */

		varsetting(static bool, auto_jump, true);
		varsetting(static bool, legit_auto_strafer, true);
		varsetting(static bool, no_crouch_cooldown, true);
		varsetting(static bool, fast_crouch_loop, true);
		varsetting(static bool, set_spotted, true);

}
