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

		// create our tabs
		static const std::vector<std::wstring> tabs = { L"AIMBOT", L"VISUALS", L"MISC", L"MANGO"};

		// create our menu
		switch (toenail::begin_window(L"m a n g o", { 100, 100 }, { 630, 500 }, &tabs))
		{
		case 0:
			toenail::checkbox(L"legit_backtrack", variables::legit_backtrack);
			toenail::checkbox(L"no_recoil", variables::no_recoil);

			toenail::checkbox(L"desync_freestanding", variables::desync_freestanding);
			toenail::checkbox(L"emotion_pitch", variables::emotion_pitch);
			toenail::checkbox(L"slide_walk", variables::slide_walk);
			toenail::checkbox(L"slow_walk", variables::slow_walk);
			toenail::checkbox(L"fakelag_adaptive", variables::fakelag_adaptive);
			toenail::checkbox(L"ragebot_test", variables::ragebot_test);
			break;
		case 1:
			toenail::checkbox(L"esp_boxes", variables::esp_boxes);
			toenail::checkbox(L"esp_corner_boxes", variables::esp_corner_boxes);
			toenail::checkbox(L"name_esp", variables::name_esp);
			toenail::checkbox(L"health_boxes", variables::health_boxes);
			toenail::checkbox(L"backtrack_stick_figure", variables::backtrack_stick_figure);
			toenail::checkbox(L"dont_render_team", variables::dont_render_team);
			toenail::checkbox(L"armour_flags", variables::armour_flags);
			toenail::checkbox(L"rivisualize_backtrack_dotsbbit", variables::visualize_backtrack_dots);
			toenail::checkbox(L"world_modulation", variables::world_modulation);
			toenail::checkbox(L"visualize_backtrack_octagon", variables::visualize_backtrack_octagon);
			toenail::checkbox(L"no_scope_lines", variables::no_scope_lines);
			toenail::checkbox(L"snap_lines", variables::snap_lines);
			toenail::checkbox(L"enemy_aim_positions", variables::enemy_aim_positions);
			toenail::checkbox(L"remove_smoke", variables::remove_smoke);
			toenail::checkbox(L"no_flash", variables::no_flash);
			toenail::checkbox(L"force_crosshair", variables::force_crosshair);
			toenail::checkbox(L"custom_crosshair", variables::custom_crosshair);
			toenail::checkbox(L"no_render_scope", variables::no_render_scope);
			toenail::checkbox(L"hand_chams", variables::hand_chams);
			toenail::checkbox(L"enemy_chams", variables::enemy_chams);
			toenail::checkbox(L"weapon_chams", variables::weapon_chams);
			
		
			break;
		case 2:
			toenail::checkbox(L"auto_jump", variables::auto_jump);
			toenail::checkbox(L"legit_auto_strafer", variables::legit_auto_strafer);

			toenail::checkbox(L"no_crouch_cooldown", variables::no_crouch_cooldown);
			toenail::checkbox(L"fast_crouch_loop", variables::fast_crouch_loop);
			toenail::checkbox(L"radar_set_spotted", variables::radar_set_spotted);
			toenail::checkbox(L"clan_tag_changer", variables::clan_tag_changer);
			toenail::checkbox(L"post_processing", variables::post_processing);
			toenail::checkbox(L"watermark", variables::watermark);
			break;

		case 3:
			
			break;
		} toenail::end_window();

		toenail::end();
	}
}