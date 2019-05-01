#include "toenail.h"

#include <memory>
#include <deque>
#include <stack>
#include <assert.h>

#include "../ap_core.h"
#include "command.h"
#include "commands.h"
#include "input.h"
#include "persistant_data.h"
#include "../misc/renderer.h"

namespace
{
	std::deque<std::shared_ptr<toenail::c_command>> command_list;
	std::stack<std::shared_ptr<toenail::c_canvas>> canvas_stack;
	toenail::c_menu_style menu_style;
	bool is_in_frame = false;
} // namespace

namespace toenail
{
	c_menu_style& get_style()
	{
		assert(!is_in_frame);
		return menu_style;
	}

	void begin()
	{
		assert(!is_in_frame);
		is_in_frame = true;

		command_list.clear();
	}
	void end()
	{
		assert(is_in_frame);
		is_in_frame = false;

		g_input.update();

		auto input = g_input;
		for (int i = command_list.size() - 1; i >= 0; i--)
			command_list[i]->update(input);

		for (const auto& cmd : command_list)
			cmd->render();
	}

	int begin_window(std::wstring_view name, ap::vec2i position, ap::vec2i size, const std::vector<std::wstring>* side_tabs)
	{
		const auto window = std::make_shared<c_window>(name, position, size, side_tabs);
		if (const auto top = top_canvas(); top)
			top->push_back(window);
		else
			command_list.push_back(window);

		push_canvas(window);
		return window->get_selected_tab();
	}
	void end_window()
	{
		pop_canvas();
	}

	bool checkbox(std::wstring_view name, bool& variable)
	{
		const auto size = ap::vec2i(15, 15);
		const auto checkbox = std::make_shared<c_checkbox>(name, calculate_position(size), size, &variable);

		top_canvas()->push_back(checkbox);
		return variable;
	}

	ap::vec2i calculate_position(ap::vec2i size)
	{
		const auto top = top_canvas();
		assert(top);

		const int inline_padding = get_style().get_property(menu_properties::inline_padding).i;

		if (top->m_child_commands.empty())
			return top->get_bounds_min() + inline_padding;

		const auto last = top->m_child_commands.back();
		return last->get_position() + ap::vec2i(0, last->get_size()[1] + 5);
	}

	std::shared_ptr<c_canvas> top_canvas()
	{
		if (canvas_stack.empty())
			return nullptr;
		return canvas_stack.top();
	}
	void push_canvas(std::shared_ptr<c_canvas> canvas)
	{
		canvas_stack.push(canvas);
	}
	void pop_canvas()
	{
		canvas_stack.pop();
	}
} // namespace toenail