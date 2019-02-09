#pragma once

#include <string>
#include <Windows.h>
#include <deque>

#include "../misc/vector.h"


namespace toenail
{
	enum keystate
	{
		released,
		pressed,
		held,

		num_keystates
	};

	class c_input
	{
	public:
		c_input();
		~c_input();
		c_input(const c_input& other);
		c_input& operator=(const c_input& other);

		void update();

		keystate get_key_state(uint8_t keycode) const;
		ap::vec2i get_mouse_position() const;
		ap::vec2i get_mouse_offset() const;
		void clear_key_state(uint8_t keycode);

		void on_key_down(uint8_t keycode);
		void on_key_up(uint8_t keycode);
		void on_char(wchar_t c);

		WNDPROC get_old_wndproc() const { return m_old_wndproc; }

	private:
		struct c_data
		{
			bool m_keyboard[256];
			std::wstring m_chars;
			ap::vec2i m_mouse_position;
		};

	private:
		c_data m_current_data,
			m_previous_data,
			m_queued_data;
		WNDPROC m_old_wndproc;
		bool m_is_copy = false;
	}; inline c_input g_input;
} // namespace toenail