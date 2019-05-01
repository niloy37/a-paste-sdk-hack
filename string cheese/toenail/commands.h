#pragma once

#include <vector>
#include <string>

#include "command.h"
#include "persistant_data.h"


namespace toenail
{
	class c_window : public c_canvas
	{
	public:
		c_window(std::wstring_view raw_name, ap::vec2i position, ap::vec2i size, const std::vector<std::wstring>* side_tabs);

		void update(c_input& input) override;
		void render() override;

		menu_commands get_command_type() override { return menu_commands::window_command; }
		ap::vec2i get_position() const override { return get_command_info().position + g_persistant_data.get_data<c_data>(get_command_info())->pos_offset; }
		ap::vec2i get_size() const override { return get_command_info().size + g_persistant_data.get_data<c_data>(get_command_info())->size_offset; }

		int get_selected_tab() const { return g_persistant_data.get_data<c_data>(get_command_info())->selected_tab; }

	private:
		struct c_data
		{
			ap::vec2i pos_offset = { 0, 0 },
				size_offset = { 0, 0 };
			bool is_dragging = false,
				is_resizing = false;
			int selected_tab = 0,
				hovered_tab = -1;
		};

	private:
		const std::vector<std::wstring>* m_side_tabs = nullptr;
	};

	class c_checkbox : public c_command
	{
	public:
		c_checkbox(std::wstring_view raw_name, ap::vec2i position, ap::vec2i size, bool* value);

		void update(c_input& input) override;
		void render() override;

		menu_commands get_command_type() override { return menu_commands::checkbox_command; }

	private:
		bool* m_value = nullptr;
	};
} // namespace toenail