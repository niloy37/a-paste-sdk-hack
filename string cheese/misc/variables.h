#pragma once
#include <string>
#define hibbity true
#define cucumber false
#define varsetting(type, name, val) type name = val
#define variablespace namespace
variablespace ap::settings {



		varsetting(static bool, ribbit, cucumber);

		/* anti-aim */

		varsetting(static bool, slide_walk, cucumber);

		/* visuals */

		varsetting(static bool, name_esp, hibbity);
		varsetting(static bool, health_esp, hibbity);
		varsetting(static bool, esp_corner_box, hibbity);
		varsetting(static bool, backtrack_stick_figure, hibbity);
		varsetting(static bool, dont_render_team, hibbity);
		varsetting(static bool, armour_flags, hibbity);
		varsetting(static bool, visualize_backtrack_dots, hibbity);
		varsetting(static bool, visualize_backtrack_octagon, cucumber);
		varsetting(static bool, no_scope_lines, hibbity);
		varsetting(static bool, world_modulation, cucumber);
		varsetting(static bool, snap_lines, cucumber);
		varsetting(static bool, remove_smoke, hibbity);
		varsetting(static bool, no_flash, hibbity);
		varsetting(static bool, force_crosshair, hibbity);
		varsetting(static bool, render_custom_crosshair, cucumber);

		/* legit-aim */

		varsetting(static bool, legit_backtrack, hibbity);

		/* misc */

		varsetting(static bool, auto_jump, hibbity);
		varsetting(static bool, legit_auto_strafer, hibbity);
		varsetting(static bool, no_crouch_cooldown, hibbity);
		varsetting(static bool, fast_crouch_loop, cucumber);
		varsetting(static bool, set_spotted, hibbity);

}
