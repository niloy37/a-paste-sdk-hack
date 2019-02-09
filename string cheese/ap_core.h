#pragma once

#include <Windows.h>

#include <string>
#include <vector>


namespace ap::core
{
	void on_attach(HINSTANCE instance);
	void on_detach();

	void setup();
	void release();

	// maybe switch output to a file when in release mode
	void log(const char* format, ...);

	HINSTANCE get_dll_handle();
	HWND get_window_handle();
} // namespace ap::core