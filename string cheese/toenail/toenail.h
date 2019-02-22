#pragma once

#include <string>
#include <vector>

#include "../misc/vector.h"

#include "style.h"


namespace toenail
{
	class c_canvas;

	c_menu_style& get_style();

	void begin();
	void end();

	int begin_window(std::wstring_view name, ap::vec2i position, ap::vec2i size, const std::vector<std::wstring>* side_tabs = nullptr);
	void end_window();

	bool checkbox(std::wstring_view name, bool& variable);

	ap::vec2i calculate_position(ap::vec2i size);

	std::shared_ptr<c_canvas> top_canvas();
	void push_canvas(std::shared_ptr<c_canvas> canvas);
	void pop_canvas();
} // namespace toenail