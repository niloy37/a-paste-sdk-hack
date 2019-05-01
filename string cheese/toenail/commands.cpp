#include "commands.h"

#include "../ap_core.h"
#include "../misc/renderer.h"

#include "toenail.h"
#include "input.h"
#include "command.h"
#include "defines.h"


namespace toenail
{
	// window
	c_window::c_window(std::wstring_view raw_name, ap::vec2i position, ap::vec2i size, const std::vector<std::wstring>* side_tabs)
	{
		setup_command_info(top_canvas(), raw_name, position, size);
		m_side_tabs = side_tabs;

		ap::vec2i min, max;
		get_dimensions(min, max);

		max += min;
		min[0] += get_command_info().style.get_property(menu_properties::window_tab_width).i;
		min[1] += get_command_info().style.get_property(menu_properties::window_title_height).i;

		set_bounds_min(min);
		set_bounds_max(max);
	}
	void c_window::update(c_input& input)
	{
		// this always has to be done first
		update_children(input);

		const auto& command_info = get_command_info();
		const auto data = g_persistant_data.get_data<c_data>(get_command_info());
		const auto screen_size = get_screen_size();

		// s t y l e ha ck  b esth ack
		const auto& style = command_info.style;
		const int title_height = style.get_property(menu_properties::window_title_height).i,
			triangle_size = style.get_property(menu_properties::window_resize_triangle_size).i;

		// get dimensions
		ap::vec2i position, size;
		get_dimensions(position, size);

		// key input
		const auto ks = input.get_key_state(VK_LBUTTON);
		if (!ks)
		{
			data->is_dragging = false;
			data->is_resizing = false;
		}

		// dragging the cheese
		if (ks == keystate::pressed && is_point_in_bounds(input.get_mouse_position(), position, position + ap::vec2i(size[0], title_height)))
		{
			input.clear_key_state(VK_LBUTTON);
			data->is_dragging = true;
		}
		if (data->is_dragging)
		{
			data->pos_offset[0] += std::clamp(input.get_mouse_offset()[0], -position[0], screen_size[0] - (position[0] + size[0]));
			data->pos_offset[1] += std::clamp(input.get_mouse_offset()[1], -position[1], screen_size[1] - (position[1] + size[1]));
		}

		// resizing
		if (ks == keystate::pressed && is_point_in_bounds(input.get_mouse_position(), position + size - triangle_size, position + size))
		{
			input.clear_key_state(VK_LBUTTON);
			data->is_resizing = true;
		}
		if (data->is_resizing)
		{
			// minimum size
			data->size_offset[0] = std::max(data->size_offset[0] + input.get_mouse_offset()[0], -command_info.size[0] + style.get_property(menu_properties::window_clamp_x).i);
			data->size_offset[1] = std::max(data->size_offset[1] + input.get_mouse_offset()[1], -command_info.size[1] + style.get_property(menu_properties::window_clamp_y).i);

			// maximum size
			data->size_offset[1] = std::min(data->size_offset[1], -command_info.size[1] + 900);
			data->size_offset[0] = std::min(data->size_offset[0], -command_info.size[0] + 1300);
		}

		// no tabs :P
		if (!m_side_tabs || m_side_tabs->empty())
			return;

		const int tab_width = style.get_property(menu_properties::window_tab_width).i;
		ap::vec2i tabs_pos = { position[0], position[1] + title_height },
			tabs_size = { tab_width, size[1] - title_height };
		const float tab_spacing = tabs_size[1] / float(m_side_tabs->size()); // yes a float

		// if mango over toe
		data->hovered_tab = -1;
		if (is_point_in_bounds(input.get_mouse_position(), tabs_pos, tabs_pos + tabs_size))
		{
			const int tab = std::clamp<int>(0, int((input.get_mouse_position()[1] - tabs_pos[1]) / tab_spacing), m_side_tabs->size() - 1);
			const auto center = tabs_pos + ap::vec2i(tabs_size[0] / 2, int(tab_spacing / 2 + tab_spacing * tab)),
				text_size = ap::renderer::get_text_size(style.get_property(menu_properties::window_tab_font).ui, command_info.name);

			// make sure the mouse is inside the actual mango
			if (!is_point_in_bounds(input.get_mouse_position(), center - text_size / 2, center + text_size / 2))
				return;

			// hoverin' over a mango
			data->hovered_tab = tab;

			// clickin' a mango
			if (ks == keystate::pressed)
			{
				input.clear_key_state(VK_LBUTTON);
				data->selected_tab = data->hovered_tab;
			}
		}
	}
	void c_window::render()
	{
		const auto& command_info = get_command_info();
		const auto data = g_persistant_data.get_data<c_data>(command_info);

		// s t y l e
		const auto& style = command_info.style;
		const int title_height = style.get_property(menu_properties::window_title_height).i,
			tab_width = style.get_property(menu_properties::window_tab_width).i,
			triangle_size = style.get_property(menu_properties::window_resize_triangle_size).i;

		// get dimensions
		ap::vec2i position, size;
		get_dimensions(position, size);

		// section specific sizes
		ap::vec2i title_bar_pos = position,
			title_bar_size = { size[0], title_height };
		ap::vec2i body_pos = { position[0], position[1] + title_height },
			body_size = { size[0], size[1] - title_height };

		// OOO0OGGggGa (booga)
		if (m_side_tabs && !m_side_tabs->empty())
		{
			body_pos[0] += tab_width;
			body_size[0] -= tab_width;
		}

		// title bar
		ap::renderer::render_filled_rect(title_bar_pos, title_bar_pos + title_bar_size, style.get_color(menu_colors::window_title_body));
		ap::renderer::render_text(title_bar_pos + title_bar_size / 2, style.get_color(menu_colors::window_title_text),
			style.get_property(menu_properties::window_title_font).ui, command_info.name, true, true);

		// body
		ap::renderer::render_filled_rect(body_pos, body_pos + body_size, style.get_color(menu_colors::window_body));

		// highlight
		ap::renderer::render_line(body_pos, body_pos + ap::vec2i(body_size[0], 0), style.get_color(menu_colors::window_highlight));

		// render the tab stuff
		if (m_side_tabs && !m_side_tabs->empty())
		{
			ap::vec2i tabs_pos = { position[0], position[1] + title_height },
				tabs_size = { tab_width, size[1] - title_height };
			const float tab_spacing = tabs_size[1] / float(m_side_tabs->size()); // yes a float

			// side tab background
			ap::renderer::render_filled_rect(tabs_pos, tabs_pos + tabs_size, style.get_color(menu_colors::window_title_body));

			// highlight
			//ap::renderer::render_line(body_pos, body_pos + ap::vec2i(0, body_size[1]), style.get_color(menu_colors::window_highlight));

			// render each tab mango
			for (size_t i = 0; i < m_side_tabs->size(); i++)
			{
				const auto& str = (*m_side_tabs)[i];

				// draw the tabs depending on whether hovered/selected/etc
				if (i == data->hovered_tab && i == data->selected_tab)
				{
					ap::vec2i tab_pos = { tabs_pos[0] + (tab_width / 2) + 1, tabs_pos[1] + int((tab_spacing / 2.f) + (i * tab_spacing)) + 1 };

					ap::renderer::render_text(tab_pos, style.get_color(menu_colors::window_tab_selected_and_hovered_text),
						style.get_property(menu_properties::window_tab_font).ui, str, true, true);
				}
				else if (i == data->selected_tab)
				{
					ap::vec2i tab_pos = { tabs_pos[0] + (tab_width / 2), tabs_pos[1] + int((tab_spacing / 2.f) + (i * tab_spacing)) };
					ap::renderer::render_text(tab_pos, style.get_color(menu_colors::window_tab_selected_text),
						style.get_property(menu_properties::window_tab_font).ui, str, true, true);
				}
				else if (i == data->hovered_tab)
				{
					ap::vec2i tab_pos = { tabs_pos[0] + (tab_width / 2) + 1, tabs_pos[1] + int((tab_spacing / 2.f) + (i * tab_spacing)) + 1 };
					ap::renderer::render_text(tab_pos, style.get_color(menu_colors::window_tab_hovered_text),
						style.get_property(menu_properties::window_tab_font).ui, str, true, true);
				}
				else
				{
					ap::vec2i tab_pos = { tabs_pos[0] + (tab_width / 2), tabs_pos[1] + int((tab_spacing / 2.f) + (i * tab_spacing)) };
					ap::renderer::render_text(tab_pos, style.get_color(menu_colors::window_tab_text), 
						style.get_property(menu_properties::window_tab_font).ui, str, true, true);
				}
			}
		}

		// triangle in bottom right
		const std::vector<ap::vec4f> triangle_vertices = {
			{ float(position[0] + size[0]), float(position[1] + size[1]), 0.f, 0.f },
			{ float(position[0] + size[0] - triangle_size), float(position[1] + size[1]), 0.f, 0.f },
			{ float(position[0] + size[0]), float(position[1] + size[1] - triangle_size), 0.f, 0.f }
		};
		ap::renderer::render_filled_polygon(triangle_vertices, style.get_color(menu_colors::window_title_body));
		ap::renderer::render_line({ position[0] + size[0] - triangle_size - 1, position[1] + size[1] }, 
			{ position[0] + size[0], position[1] + size[1] - triangle_size - 1 }, style.get_color(menu_colors::window_outline));


		// inline
		ap::renderer::render_empty_rect(position - ap::vec2i(1), position + size + ap::vec2i(1), style.get_color(menu_colors::window_inline));

		// outline
		ap::renderer::render_empty_rect(position - ap::vec2i(2), position + size + ap::vec2i(2), style.get_color(menu_colors::window_outline));

		//ap::renderer::render_empty_rect(get_bounds_min(), get_bounds_max(), ap::rgba8::GREEN());

		// this always has to be done last
		render_children();
	}

	// checkbox
	c_checkbox::c_checkbox(std::wstring_view raw_name, ap::vec2i position, ap::vec2i size, bool* value)
	{
		m_value = value;
		setup_command_info(top_canvas(), raw_name, position, size);
	}
	void c_checkbox::update(c_input& input)
	{
		const auto position = get_position(),
			size = get_size();
		if (input.get_key_state(VK_LBUTTON) == keystate::pressed && is_point_in_bounds(input.get_mouse_position(), position, position + size))
		{
			*m_value = !(*m_value);
		}
	}
	void c_checkbox::render()
	{
		const auto position = get_position(),
			size = get_size();
		const auto& command_info = get_command_info();
		ap::renderer::render_filled_rect(position, position + size, *m_value ? ap::rgba8::GREEN() : ap::rgba8::RED());
		ap::renderer::render_text(position + ap::vec2i(size[0] + 5, 0), ap::rgba8::WHITE(), toenail::window_title_font, command_info.name, false, false);
	}
} // namespace toenail