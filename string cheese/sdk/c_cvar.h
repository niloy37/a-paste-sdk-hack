#pragma once

#include <stdint.h>

#include "../misc/vmt.h"
#include "../misc/color.h"


namespace ap::sdk
{
	enum cvar_flags {
		fcvar_none = 0,
		fcvar_unregistered = (1 << 0),
		fcvar_developmentonly = (1 << 1),
		fcvar_gamedll = (1 << 2),
		fcvar_clientdll = (1 << 3),
		fcvar_hidden = (1 << 4),
		fcvar_protected = (1 << 5),
		fcvar_sponly = (1 << 6),
		fcvar_archive = (1 << 7),
		fcvar_notify = (1 << 8),
		fcvar_userinfo = (1 << 9),
		fcvar_printableonly = (1 << 10),
		fcvar_unlogged = (1 << 11),
		fcvar_never_as_string = (1 << 12),
		fcvar_replicated = (1 << 13),
		fcvar_cheat = (1 << 14),
		fcvar_ss = (1 << 15),
		fcvar_demo = (1 << 16),
		fcvar_dontrecord = (1 << 17),
		fcvar_ss_added = (1 << 18),
		fcvar_release = (1 << 19),
		fcvar_reload_materials = (1 << 20),
		fcvar_reload_textures = (1 << 21),
		fcvar_not_connected = (1 << 22),
		fcvar_material_system_thread = (1 << 23),
		fcvar_archive_xbox = (1 << 24),
		fcvar_accessible_from_threads = (1 << 25),
		fcvar_server_can_execute = (1 << 28),
		fcvar_server_cannot_query = (1 << 29),
		fcvar_clientcmd_can_execute = (1 << 30),
		fcvar_meme_dll = (1 << 31),
		fcvar_material_thread_mask = (fcvar_reload_materials | fcvar_reload_textures | fcvar_material_system_thread)
	};
	class c_convar
	{
	public:
		char pad_0x0000[0x4];
		c_convar * pNext;
		int bRegistered;
		char* pszName;
		char* pszHelpString;
		int nFlags;
		char pad_0x0018[0x4];
		c_convar* pParent;
		char* pszDefaultValue;
		char* strString;
		int StringLength;
		float fValue;
		int nValue;
		int bHasMin;
		float fMinVal;
		int bHasMax;
		float fMaxVal;
		void *fnChangeCallback;

		float get_float(void) const
		{
			const int temp = *(int*)(&fValue);
			const int temp_result = int(temp ^ uint32_t(this));

			return *reinterpret_cast<const float*>(&temp_result);
		}
		int get_int(void) const
		{
			const int temp = *(int*)(&fValue);
			const int temp_result = int(temp ^ uint32_t(this));

			return *static_cast<const int*>(&temp_result);
		}
		bool get_bool(void) const
		{
			return !!get_int();
		}

		void set_value(const char* value)
		{
			using fn = void(__thiscall*)(void*, const char*);
			return vmt::get_vfunc<fn>(this, 14)(this, value);
		}

		void set_value(int value)
		{
			using fn = void(__thiscall*)(void*, int);
			return vmt::get_vfunc<fn>(this, 16)(this, value);
		}

	};//Size=0x0048

	class c_cvar
	{
	public:
		// IAppSystem https://github.com/alliedmodders/hl2sdk/blob/csgo/public/appframework/IAppSystem.h
		virtual bool func0() = 0;
		virtual bool func1() = 0;
		virtual bool func2() = 0;
		virtual bool func3() = 0;
		virtual bool func4() = 0;
		virtual bool func5() = 0;
		virtual bool func6() = 0;
		virtual bool func7() = 0;
		virtual bool func8() = 0;

		// ICvar https://github.com/alliedmodders/hl2sdk/blob/csgo/public/icvar.h
		virtual void func9() = 0;

		virtual void register_con_command(void *pCommandBase) = 0;
		virtual void unregister_con_command(void *pCommandBase) = 0;
		virtual void func12() = 0;

		virtual const char* get_command_line_value(const char *pVariableName) = 0;

		virtual void func14() = 0;
		virtual void func15() const = 0;
		virtual c_convar* find_var(const char *var_name) = 0;
		virtual const c_convar* find_var(const char *var_name) const = 0;
		virtual void func18() = 0;
		virtual void func19() = 0;

		virtual void func20() = 0;
		virtual void func21() = 0;
		virtual void func22() = 0;

		virtual void func23() = 0;
		virtual void func24() = 0;
		virtual void ConsoleColorPrintf(const rgba8& clr, const char *pFormat, ...) const = 0;
		virtual void ConsolePrintf(const char *pFormat, ...) const = 0;
		virtual void ConsoleDPrintf(const char *pFormat, ...) const = 0;

		virtual void func28() = 0;
		virtual void func29() = 0;

	private:
	};
} // namespace ap::sdk