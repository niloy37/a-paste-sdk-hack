#include "menu.h"
#include "../../string cheese/variables.h"
namespace ap::menu {
	void run() {
		if (static bool is_init = false; !is_init) {
			is_init = true;

			auto& style = toenail::get_style();

			// fonts
			static const auto xolonium_16 = ap::renderer::create_font("xolonium", 11, 400, 0, 0, renderer::fontflag::dropshadow);
			static const auto cherry_42 = renderer::create_font("cherryfont", 49, 400, 0, 0, renderer::fontflag::antialias);

			// colors
			style.set_color(toenail::menu_colors::window_body, rgba8(40));
			style.set_color(toenail::menu_colors::window_inline, rgba8(60));
			style.set_color(toenail::menu_colors::window_outline, rgba8(20));
			style.set_color(toenail::menu_colors::window_title_body, rgba8(30));
			style.set_color(toenail::menu_colors::window_title_text, rgba8(200));
			style.set_color(toenail::menu_colors::window_highlight, rgba8(244, 69, 66));
			style.set_color(toenail::menu_colors::window_tab_text, rgba8(100));
			style.set_color(toenail::menu_colors::window_tab_hovered_text, rgba8(150));
			style.set_color(toenail::menu_colors::window_tab_selected_text, rgba8(230));
			style.set_color(toenail::menu_colors::window_tab_selected_and_hovered_text, rgba8(255));

			// properties
			style.set_property(toenail::menu_properties::inline_padding, 12);
			style.set_property(toenail::menu_properties::groupbox_padding_width, 12);

			style.set_property(toenail::menu_properties::window_title_height, 18);
			style.set_property(toenail::menu_properties::window_tab_width, 100);
			style.set_property(toenail::menu_properties::window_resize_triangle_size, 6);
			style.set_property(toenail::menu_properties::window_clamp_x, 630);
			style.set_property(toenail::menu_properties::window_clamp_y, 500);
			style.set_property(toenail::menu_properties::window_title_font, xolonium_16);
			style.set_property(toenail::menu_properties::window_tab_font, xolonium_16);
		}

		// start
		toenail::begin();

		// menu toggle
		if (toenail::g_input.get_key_state(VK_INSERT) == toenail::pressed)
		{
			menu_open = !menu_open;
		}
		// if menu isn't enabled
		if (!menu_open)
		{
			toenail::end();
			return;
		}
		static bool test_var = false;
		//if (hovered)
		//{
		//	ap::vec2i mouse_pos = toenail::g_input.get_mouse_position();
		//	std::wstring text = L"tooltip";
		//	ap::vec2i text_size = ap::renderer::get_text_size(ap::globals::group_font, text);
		//	ap::renderer::render_filled_rect(mouse_pos - ap::vec2i(0, text_size[1]), { text_size[0] + 5, text_size[1] - 2 }, ap::rgba8::BLACK());
		//}
		// create our tabs
		static const std::vector<std::wstring> tabs = { L"A I M B O T", L"V I S U A L S", L"M I S C", L"M A N G O"};

		// create our menu
		switch (toenail::begin_window(L"m a n g o", { 100, 100 }, { 630, 500 }, &tabs))
		{
		case 0:
			toenail::checkbox(L"legit_backtrack", variables::legit::get().legit_backtrack, false);
			toenail::checkbox(L"no_recoil", variables::rage::get().no_recoil, false);

			toenail::checkbox(L"desync_freestanding", variables::antiaim::get().desync_freestanding, false);
			toenail::checkbox(L"emotion_pitch", variables::antiaim::get().emotion_pitch, false);
			toenail::checkbox(L"slide_walk", variables::antiaim::get().slide_walk, false);
			toenail::checkbox(L"slow_walk", variables::antiaim::get().slow_walk, false);
			toenail::checkbox(L"fakelag_adaptive", variables::antiaim::get().fakelag_adaptive, false);
			toenail::checkbox(L"ragebot_head_only", variables::rage::get().ragebot_head_only, false);
			toenail::checkbox(L"ragebot_test", variables::rage::get().ragebot_test, false);
			toenail::checkbox(L"silent_aim", variables::rage::get().silent_aim, false);
			toenail::checkbox(L"auto_stop", variables::rage::get().auto_stop, false);
			break;
		case 1:
			toenail::checkbox(L"esp_boxes", variables::visuals::get().esp_boxes, false);
			toenail::checkbox(L"esp_corner_boxes", variables::visuals::get().esp_corner_boxes, false);
			toenail::checkbox(L"name_esp", variables::visuals::get().name_esp, false);
			toenail::checkbox(L"health_boxes", variables::visuals::get().health_boxes, false);
			toenail::checkbox(L"backtrack_stick_figure", variables::visuals::get().backtrack_stick_figure, false);
			toenail::checkbox(L"dont_render_team", variables::visuals::get().dont_render_team, false);
			toenail::checkbox(L"armour_flags", variables::visuals::get().armour_flags, false);
			toenail::checkbox(L"rivisualize_backtrack_dotsbbit", variables::visuals::get().visualize_backtrack_dots, false);
			toenail::checkbox(L"world_modulation", variables::visuals::get().world_modulation, false);
			toenail::checkbox(L"visualize_backtrack_octagon", variables::visuals::get().visualize_backtrack_octagon, false);
			toenail::checkbox(L"no_scope_lines", variables::visuals::get().no_scope_lines, false);
			toenail::checkbox(L"snap_lines", variables::visuals::get().snap_lines, false);
			toenail::checkbox(L"enemy_aim_positions", variables::visuals::get().enemy_aim_positions, false);
			toenail::checkbox(L"remove_smoke", variables::visuals::get().remove_smoke, false);
			toenail::checkbox(L"no_flash", variables::visuals::get().no_flash, false);
			toenail::checkbox(L"force_crosshair", variables::visuals::get().force_crosshair, false);
			toenail::checkbox(L"custom_crosshair", variables::visuals::get().custom_crosshair, false);
			toenail::checkbox(L"no_render_scope", variables::visuals::get().no_render_scope, false);
			toenail::checkbox(L"hand_chams", variables::visuals::get().hand_chams, false);
			toenail::checkbox(L"enemy_chams", variables::visuals::get().enemy_chams, false);
			toenail::checkbox(L"weapon_chams", variables::visuals::get().weapon_chams, false);
			
		
			break;
		case 2:
			toenail::checkbox(L"auto_jump", variables::misc::get().auto_jump, false);
			toenail::checkbox(L"legit_auto_strafer", variables::misc::get().legit_auto_strafer, false);

			toenail::checkbox(L"no_crouch_cooldown", variables::misc::get().no_crouch_cooldown, false);
			toenail::checkbox(L"fast_crouch_loop", variables::misc::get().fast_crouch_loop, false);
			toenail::checkbox(L"radar_set_spotted", variables::misc::get().radar_set_spotted, false);
			toenail::checkbox(L"clan_tag_changer", variables::misc::get().clan_tag_changer, false);
			toenail::checkbox(L"post_processing", variables::misc::get().post_processing, false);
			toenail::checkbox(L"watermark", variables::misc::get().watermark, false);
			break;

		case 3:
			
			break;
		} toenail::end_window();

		toenail::end();
	}
}