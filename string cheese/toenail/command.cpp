#include "command.h"

#include "toenail.h"


namespace toenail
{
	std::wstring filter_name(std::wstring_view str)
	{
		std::wstring new_str;
		for (size_t i = 0; i < str.size(); i++)
		{
			if (i < str.size() - 1 && str[i] == '#' && str[i + 1] == '#')
				return new_str;

			new_str += str[i];
		}

		return new_str;
	}

	void c_command::get_dimensions(ap::vec2i& pos, ap::vec2i& size) const
	{
		if (!m_command_info.parent_canvas)
		{
			pos = get_position();
			size = get_size();
			return;
		}
		
		m_command_info.parent_canvas->get_dimensions(pos, size);
		pos += get_position();
		size = get_size();
	}
	void c_command::setup_command_info(std::shared_ptr<c_canvas> parent, std::wstring_view raw_name, ap::vec2i position, ap::vec2i size)
	{
		m_command_info.parent_canvas = parent;
		m_command_info.raw_name = raw_name;
		m_command_info.name = filter_name(std::wstring(raw_name));
		m_command_info.position = position;
		m_command_info.size = size;
		m_command_info.command_type = get_command_type();
		m_command_info.style = get_style();
	}

	void c_canvas::update_children(c_input& input)
	{
		for (int i = m_child_commands.size() - 1; i >= 0; i--)
			m_child_commands[i]->update(input);
	}
	void c_canvas::render_children()
	{
		for (const auto& cmd : m_child_commands)
			cmd->render();
	}
} // namespace toenail