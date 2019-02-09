#include "input.h"

#include <algorithm>

#include "../ap_core.h"


namespace
{
	// https://docs.microsoft.com/en-us/windows/desktop/inputdev/keyboard-input-notifications
	LRESULT CALLBACK wnd_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
	{
		switch (message)
		{
		case WM_KEYDOWN:
			toenail::g_input.on_key_down(w_param);
			break;
		case WM_KEYUP:
			toenail::g_input.on_key_up(w_param);
			break;
		case WM_LBUTTONDOWN:
			toenail::g_input.on_key_down(VK_LBUTTON);
			break;
		case WM_LBUTTONUP:
			toenail::g_input.on_key_up(VK_LBUTTON);
			break;
		case WM_CHAR:
			toenail::g_input.on_char(w_param);
			break;
		}

		return CallWindowProc(toenail::g_input.get_old_wndproc(), hwnd, message, w_param, l_param);
	}
} // namespace

namespace toenail
{
	c_input::c_input()
	{
		m_old_wndproc = WNDPROC(SetWindowLongPtr(ap::core::get_window_handle(), GWL_WNDPROC, LONG_PTR(wnd_proc)));
	}
	c_input::~c_input()
	{
		if (!m_is_copy)
			SetWindowLongPtr(ap::core::get_window_handle(), GWL_WNDPROC, LONG_PTR(m_old_wndproc));
	}
	c_input::c_input(const c_input& other)
	{
		memcpy(this, &other, sizeof(c_input));
		m_is_copy = true;
	}
	c_input& c_input::operator=(const c_input& other)
	{
		memcpy(this, &other, sizeof(c_input));
		m_is_copy = true;
		return *this;
	}

	void c_input::update()
	{
		m_previous_data = m_current_data;

		const auto window_handle = ap::core::get_window_handle();

		// get client area
		RECT client_size;
		GetClientRect(window_handle, &client_size);
		const auto screen_width = client_size.right - client_size.left,
			screen_height = client_size.bottom - client_size.top;

		// get mouse position
		POINT mouse_position;
		GetCursorPos(&mouse_position);
		ScreenToClient(window_handle, &mouse_position);

		// if mouse went out of client area
		if (mouse_position.x < 0L || mouse_position.x > screen_width || mouse_position.y < 0L || mouse_position.y > screen_height)
		{
			for (size_t i = 0; i < 256; i++)
				m_queued_data.m_keyboard[i] = false;

			// clamp mouse position to client area
			mouse_position.x = std::clamp(mouse_position.x, 0L, screen_width);
			mouse_position.y = std::clamp(mouse_position.y, 0L, screen_height);
		}

		m_queued_data.m_mouse_position = { mouse_position.x, mouse_position.y };
		m_current_data = m_queued_data;

		m_queued_data.m_chars.clear();
	}

	keystate c_input::get_key_state(uint8_t keycode) const
	{
		if (m_current_data.m_keyboard[keycode] && !m_previous_data.m_keyboard[keycode])
			return keystate::pressed;
		if (m_current_data.m_keyboard[keycode] && m_previous_data.m_keyboard[keycode])
			return keystate::held;

		return keystate::released;
	}
	ap::vec2i c_input::get_mouse_position() const
	{
		return m_current_data.m_mouse_position;
	}
	ap::vec2i c_input::get_mouse_offset() const
	{
		return m_current_data.m_mouse_position - m_previous_data.m_mouse_position;
	}

	void c_input::clear_key_state(uint8_t keycode)
	{
		m_current_data.m_keyboard[keycode] = false;
	}

	void c_input::on_key_down(uint8_t keycode)
	{
		m_queued_data.m_keyboard[keycode] = true;
	}
	void c_input::on_key_up(uint8_t keycode)
	{
		m_queued_data.m_keyboard[keycode] = false;
	}
	void c_input::on_char(wchar_t c)
	{
		m_queued_data.m_chars.push_back(c);
	}
} // namespace toenail