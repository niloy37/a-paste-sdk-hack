#include "menu.h"

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
			style.set_property(toenail::menu_properties::window_tab_font, cherry_42);
		}

		// start
		toenail::begin();

		// set the yeet x & y clamping min to 200
		toenail::get_style().push_property(toenail::menu_properties::window_clamp_x, 200);
		toenail::get_style().push_property(toenail::menu_properties::window_clamp_y, 200);

		//// create "ribbit" window
		//toenail::begin_window(L"ribbit", { 200, 200 }, { 200, 200 });
		//toenail::end_window();

		// grab the old x & y min clamp
		toenail::get_style().pop_property(toenail::menu_properties::window_clamp_x);
		toenail::get_style().pop_property(toenail::menu_properties::window_clamp_y);

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

		// create our tabs
		static const std::vector<std::wstring> tabs = { L"A" /* RAGEBOT */, L"D" /* VISUALS */, L"G" /* SETTINGS */, L"B" /* SKINS */ };

		// create our menu
		switch (toenail::begin_window(L"m a n g o", { 100, 100 }, { 630, 500 }, &tabs))
		{
		case 0:
			break;
		case 1:
			toenail::begin_window(L"ribbit", { 200, 200 }, { 200, 200 });
			toenail::end_window();
			break;
		case 2:

			break;

		case 3:

			break;
		} toenail::end_window();

		toenail::end();
	}
}