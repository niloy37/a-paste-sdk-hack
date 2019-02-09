#include "ap_core.h"

#include <iostream>
#include <vector>
#include "misc/interfaces.h"
#include "misc/hooks.h"
#include "misc/vector.h"
#include "sdk/offsets.h"
#include "sdk/c_cvar.h"


namespace
{
	HINSTANCE g_dll_handle = nullptr;

	DWORD WINAPI main_thread(void*)
	{
		ap::core::setup();

		while (!GetAsyncKeyState(VK_HOME)) 
			Sleep(100);

		ap::core::release();
		return 0;
	}

	std::vector<std::string> logs;
} // namespace

namespace ap::core
{
	void on_attach(HINSTANCE instance)
	{
		g_dll_handle = instance;
		CreateThread(nullptr, 0, main_thread, nullptr, 0, nullptr);
	}
	void on_detach()
	{

	}

	void setup()
	{
#ifdef NDEBUG
		log("injected");
#else
		log("injected debug mode");
#endif

		interfaces::setup();
		offsets::setup();
		hooks::setup();
	}
	void release()
	{
		log("uninjected");

		hooks::release();
		Sleep(200);
		FreeLibraryAndExitThread(g_dll_handle, 0);
	}

	// will break with more than 1024 chars but... why would u ever do that
	void log(const char* format, ...)
	{
		char buffer[1024];
		va_list arg_list;
		va_start(arg_list, format);
		vsnprintf_s(buffer, 1024, format, arg_list);
		va_end(arg_list);

		if (interfaces::cvar)
		{
			for (const auto& str : logs)
			{
				interfaces::cvar->ConsoleColorPrintf(rgba8(150), "[");
				interfaces::cvar->ConsoleColorPrintf(rgba8(100, 200, 255), "a paste");
				interfaces::cvar->ConsoleColorPrintf(rgba8(150), "] ");
				interfaces::cvar->ConsoleColorPrintf(rgba8(100, 150, 255), "%s\n", str.c_str());
			} logs.clear();

			interfaces::cvar->ConsoleColorPrintf(rgba8(150), "[");
			interfaces::cvar->ConsoleColorPrintf(rgba8(100, 200, 255), "a paste");
			interfaces::cvar->ConsoleColorPrintf(rgba8(150), "] ");
			interfaces::cvar->ConsoleColorPrintf(rgba8(100, 150, 255), "%s\n", buffer);
		}
		else
			logs.push_back(buffer);
	}

	HINSTANCE get_dll_handle()
	{
		return g_dll_handle;
	}
	HWND get_window_handle()
	{
		static const auto hwnd = FindWindow(nullptr, "Counter-Strike: Global Offensive");
		return hwnd;
	}
} // namespace ap::core