#include "menu.h"

#include "toenail.h"
#include "defines.h"

#include "../ap_core.h"
#include "../misc/renderer.h"


namespace ap::menu
{
	void run()
	{
		if (static bool is_init = false; !is_init)
		{
			is_init = true;

			auto& style = toenail::get_style();

			static const auto xolonium_16 = ap::renderer::create_font("Xolonium", 16, 400, 0, 0, renderer::fontflag::antialias);
			static const auto cherry_42 = renderer::create_font("cherryfont", 42, 400, 0, 0, renderer::fontflag::antialias);

			// colors
			style.set_color(toenail::menu_colors::window_body, rgba8(40));
			style.set_color(toenail::menu_colors::window_inline, rgba8(60));
			style.set_color(toenail::menu_colors::window_outline, rgba8(20));
			style.set_color(toenail::menu_colors::window_title_body, rgba8(30));
			style.set_color(toenail::menu_colors::window_title_text, rgba8(200));
			style.set_color(toenail::menu_colors::window_highlight, rgba8(255, 255, 0));
			style.set_color(toenail::menu_colors::window_tab_text, rgba8(200));
			style.set_color(toenail::menu_colors::window_tab_hovered_text, rgba8(255));
			style.set_color(toenail::menu_colors::window_tab_selected_text, rgba8(100));

			// properties
			style.set_property(toenail::menu_properties::inline_padding, 12);

			style.set_property(toenail::menu_properties::window_title_height, 18);
			style.set_property(toenail::menu_properties::window_tab_width, 80);
			style.set_property(toenail::menu_properties::window_resize_triangle_size, 10);
			style.set_property(toenail::menu_properties::window_title_font, xolonium_16);
			style.set_property(toenail::menu_properties::window_tab_font, cherry_42);
		}

		toenail::begin();

		static const std::vector<std::wstring> tabs = { L"A" /* RAGEBOT */, L"D" /* VISUALS */, L"G" /* SETTINGS */, L"B" /* SKINS */ };
		switch (toenail::begin_window(L"a paste", { 100, 100 }, { 800, 450 }, &tabs))
		{
		case 0:
			toenail::checkbox(L"test");
			break;
		case 1:

			break;
		case 2:

			break;
		} toenail::end_window();

		toenail::end();
	}
}