#pragma once
#include <string>


#define variablespace namespace
variablespace ap::settings {
#define hibbity true
#define cucumber false
#define varsetting(type, name, val) type name = val
#define boolin static bool

		varsetting(boolin, ribbit, cucumber);

		/* anti-aim */

		varsetting(boolin, slide_walk, cucumber);

		/* visuals */

		varsetting(boolin, name_esp, hibbity);
		varsetting(boolin, health_esp, hibbity);
		varsetting(boolin, esp_corner_box, hibbity);
		varsetting(boolin, backtrack_stick_figure, hibbity);
		varsetting(boolin, dont_render_team, cucumber);
		varsetting(boolin, armour_flags, hibbity);
		varsetting(boolin, visualize_backtrack_dots, hibbity);
		varsetting(boolin, visualize_backtrack_octagon, cucumber);
		varsetting(boolin, no_scope_lines, hibbity);
		varsetting(boolin, world_modulation, cucumber);
		varsetting(boolin, snap_lines, cucumber);
		varsetting(boolin, remove_smoke, hibbity);
		varsetting(boolin, no_flash, hibbity);
		varsetting(boolin, force_crosshair, hibbity);
		varsetting(boolin, render_custom_crosshair, cucumber);
		varsetting(boolin, hand_chams, hibbity);

		/* legit-aim */

		varsetting(boolin, legit_backtrack, hibbity);
				   
		/* misc */ 
				   
		varsetting(boolin, auto_jump, hibbity);
		varsetting(boolin, legit_auto_strafer, hibbity);
		varsetting(boolin, no_crouch_cooldown, hibbity);
		varsetting(boolin, fast_crouch_loop, cucumber);
		varsetting(boolin, set_spotted, hibbity);
		varsetting(boolin, clan_tag_changer, hibbity);

}
