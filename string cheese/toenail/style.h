#pragma once

#include <stack>
#include <array>

#include "../misc/color.h"


namespace toenail
{
	enum menu_colors
	{
		window_body,
		window_inline,
		window_outline,
		window_title_body,
		window_title_text,
		window_tab_text,
		window_tab_hovered_text,
		window_tab_selected_text,
		window_highlight,

		num_colors
	}; // rgba8
	enum menu_properties
	{
		inline_padding,

		window_title_height,
		window_tab_width,
		window_resize_triangle_size,
		window_title_font,
		window_tab_font,

		num_properties
	}; // menu_property

	union menu_property
	{
		menu_property(int value) : i(value) { }
		menu_property(float value) : f(value) { }
		menu_property(unsigned int value) : ui(value) { }

		int i;
		float f;
		unsigned int ui;
	};

	class c_menu_style
	{
	public:
		c_menu_style()
		{
			for (size_t i = 0; i < menu_colors::num_colors; i++)
				colors[i] = std::stack<ap::rgba8>({ ap::rgba8::WHITE() });

			for (size_t i = 0; i < menu_properties::num_properties; i++)
				properties[i] = std::stack<menu_property>({ 10 });
		}

		const ap::rgba8 get_color(menu_colors i) const { return colors[i].top(); }
		const menu_property get_property(menu_properties i) const { return properties[i].top(); }

		void set_color(menu_colors i, ap::rgba8 color) { colors[i] = std::stack<ap::rgba8>({ color }); }
		void set_property(menu_properties i, menu_property prop) { properties[i] = std::stack<menu_property>({ prop }); }

		void push_color(menu_colors i, ap::rgba8 color) { colors[i].push(color); }
		void push_property(menu_properties i, menu_property prop) { properties[i].push(prop); }

		void pop_color(menu_colors i) { colors[i].pop(); }
		void pop_property(menu_properties i) { properties[i].pop(); }

	private:
		std::array<std::stack<ap::rgba8>, menu_colors::num_colors> colors;
		std::array<std::stack<menu_property>, menu_properties::num_properties> properties;
	};
} // namespace toenail