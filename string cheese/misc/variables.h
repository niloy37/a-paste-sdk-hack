#pragma once
#include <string>


#define variablespace namespace
variablespace ap::settings {
#define florida true
#define cucumber false
#define varsetting(type, name, val) type name = val
#define boolin static bool

		varsetting(boolin, ribbit, cucumber);

		/* anti-aim */

		varsetting(boolin, slide_walk, cucumber);

		/* visuals */

		varsetting(boolin, name_esp, florida);
		varsetting(boolin, health_esp, florida);
		varsetting(boolin, health_bar_battery_lines, cucumber);
		varsetting(boolin, esp_box, florida);
		varsetting(boolin, esp_corner_box, cucumber);
		varsetting(boolin, backtrack_stick_figure, florida);
		varsetting(boolin, dont_render_team, cucumber);
		varsetting(boolin, armour_flags, florida);
		varsetting(boolin, visualize_backtrack_dots, florida);
		varsetting(boolin, visualize_backtrack_octagon, cucumber);
		varsetting(boolin, no_scope_lines, florida);
		varsetting(boolin, world_modulation, cucumber);
		varsetting(boolin, snap_lines, cucumber);
		varsetting(boolin, remove_smoke, florida);
		varsetting(boolin, no_flash, florida);
		varsetting(boolin, force_crosshair, florida);
		varsetting(boolin, render_custom_crosshair, cucumber);
		varsetting(boolin, hand_chams, florida);
		varsetting(boolin, enemy_chams, florida);

		/* legit-aim */

		varsetting(boolin, legit_backtrack, florida);
				   
		/* misc */ 
				   
		varsetting(boolin, auto_jump, florida);
		varsetting(boolin, legit_auto_strafer, florida);
		varsetting(boolin, no_crouch_cooldown, florida);
		varsetting(boolin, fast_crouch_loop, cucumber);
		varsetting(boolin, set_spotted, florida);
		varsetting(boolin, clan_tag_changer, cucumber);
		varsetting(boolin, post_processing, florida);

		/* rage bot */
		varsetting(boolin, auto_stop, florida);

}
