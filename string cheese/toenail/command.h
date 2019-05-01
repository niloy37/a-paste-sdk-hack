#pragma once

#include <memory>
#include <string>
#include <stack>
#include <vector>

#include "../misc/vector.h"
#include "input.h"
#include "style.h"


namespace toenail
{
	// example filter_name("TEST##DAB") -> "TEST"
	std::wstring filter_name(std::wstring_view str);

	enum menu_commands
	{
		window_command,
		checkbox_command,
		num_commands
	};

	class c_canvas;
	struct c_command_info
	{
		std::wstring raw_name,
			name;
		ap::vec2i position,
			size;
		std::shared_ptr<c_canvas> parent_canvas = nullptr;
		menu_commands command_type;
		c_menu_style style;
	};

	class c_command
	{
	public:
		virtual ~c_command() {}

		virtual void update(c_input& input) {}
		virtual void render() {}

		virtual ap::vec2i get_position() const { return m_command_info.position; }
		virtual ap::vec2i get_size() const { return m_command_info.size; }

		virtual bool is_canvas() { return false; }
		virtual menu_commands get_command_type() = 0;

		void get_dimensions(ap::vec2i& pos, ap::vec2i& size) const;
		void setup_command_info(std::shared_ptr<c_canvas> parent, std::wstring_view raw_name, ap::vec2i position, ap::vec2i size);

		c_command_info& get_command_info() { return m_command_info; }
		const c_command_info& get_command_info() const { return m_command_info; }

	private:
		c_command_info m_command_info;
	};

	class c_canvas : public c_command
	{
		friend ap::vec2i calculate_position(ap::vec2i size);
	public:
		virtual ~c_canvas() {}

		void push_back(std::shared_ptr<c_command> command) { m_child_commands.push_back(command); }

		void update_children(c_input& input);
		void render_children();

		ap::vec2i get_bounds_min() const { return m_bounds_min; }
		void set_bounds_min(ap::vec2i min) { m_bounds_min = min; }

		ap::vec2i get_bounds_max() const { return m_bounds_max; }
		void set_bounds_max(ap::vec2i max) { m_bounds_max = max; }

		bool is_canvas() override { return true; }

	private:
		std::deque<std::shared_ptr<c_command>> m_child_commands;
		ap::vec2i m_bounds_min,
			m_bounds_max;
	};
} // namespace toenail