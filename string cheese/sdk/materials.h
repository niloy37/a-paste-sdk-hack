#pragma once

#include "../misc/vmt.h"
#include "../misc/vector.h"
#include "../misc/color.h"


namespace ap::sdk
{
	enum MATERIAL_VAR_FLAG
	{
		MATERIAL_VAR_DEBUG = (1 << 0),
		MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
		MATERIAL_VAR_NO_DRAW = (1 << 2),
		MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

		MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
		MATERIAL_VAR_VERTEXALPHA = (1 << 5),
		MATERIAL_VAR_SELFILLUM = (1 << 6),
		MATERIAL_VAR_ADDITIVE = (1 << 7),
		MATERIAL_VAR_ALPHATEST = (1 << 8),

		MATERIAL_VAR_ZNEARER = (1 << 10),
		MATERIAL_VAR_MODEL = (1 << 11),
		MATERIAL_VAR_FLAT = (1 << 12),
		MATERIAL_VAR_NOCULL = (1 << 13),
		MATERIAL_VAR_NOFOG = (1 << 14),
		MATERIAL_VAR_IGNOREZ = (1 << 15),
		MATERIAL_VAR_DECAL = (1 << 16),
		MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE

		MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
		MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
		MATERIAL_VAR_TRANSLUCENT = (1 << 21),
		MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
		MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
		MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
		MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
		MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
		MATERIAL_VAR_HALFLAMBERT = (1 << 27),
		MATERIAL_VAR_WIREFRAME = (1 << 28),
		MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
		MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
		MATERIAL_VAR_VERTEXFOG = (1 << 31),
	};

	using material_handle = unsigned short;

	class c_key_values
	{
	public:
		void* operator new(size_t alloc_size)
		{
			static PVOID key_values_system;
			if (!key_values_system)
			{
				using fn = void*(__cdecl*)();
				key_values_system = fn(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "KeyValuesSystem"))();
			}

			using fn = void*(__thiscall*)(void*, size_t);
			return vmt::get_vfunc<fn>(key_values_system, 1)(key_values_system, alloc_size);
		}
		void operator delete(void* address)
		{
			static PVOID key_values_system;
			if (!key_values_system)
			{
				using fn = void*(__cdecl*)();
				key_values_system = fn(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "KeyValuesSystem"))();
			}

			using fn = void(__thiscall*)(void*, void*);
			vmt::get_vfunc<fn>(key_values_system, 2)(key_values_system, address);
		}

		char pad[0x20];
	};

	class c_material_var
	{
	public:
		virtual void* GetTextureValue(void) = 0;
		virtual char const* GetName(void) const = 0;
	};

	class c_material
	{
	public:
		void set_material_var_flag(MATERIAL_VAR_FLAG flag, bool on)
		{
			using fn = void(__thiscall*)(void*, MATERIAL_VAR_FLAG, bool);
			return vmt::get_vfunc<fn>(this, 29)(this, flag, on);
		}
		void alpha_modulate(float alpha)
		{
			using fn = void(__thiscall*)(void*, float);
			return vmt::get_vfunc<fn>(this, 27)(this, alpha);
		}
		void color_modulate(rgbaf color)
		{
			using fn = void(__thiscall*)(void*, float, float, float);
			return vmt::get_vfunc<fn>(this, 28)(this, color[0], color[1], color[2]);
		}
		const char* get_texture_group_name()
		{
			using fn = const char*(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 1)(this);
		}
		const char* get_material_name()
		{
			using fn = const char*(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 0)(this);
		}
		void increment_ref_count()
		{
			using fn = void(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 12)(this);
		}
		void decrement_ref_count()
		{
			using fn = void(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 13)(this);
		}
		int get_shader_param_count()
		{
			using fn = int(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 40)(this);
		}
		c_material_var** get_shader_params()
		{
			using fn = c_material_var**(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 41)(this);
		}

	private:
	};

	class c_material_system
	{
	public:
		c_material* create_material(const char* c, c_key_values* k)
		{
			using fn = c_material*(__thiscall*)(void*, const char*, c_key_values*);
			return vmt::get_vfunc<fn>(this, 83)(this, c, k);
		}
		c_material* find_material(char const* material_name, const char* texture_group_name, bool complain = true, const char* complain_prefix = nullptr)
		{
			using fn = c_material*(__thiscall*)(void*, char const*, const char*, bool, const char*);
			return vmt::get_vfunc<fn>(this, 84)(this, material_name, texture_group_name, complain, complain_prefix);
		}

		material_handle first_material()
		{
			using fn = material_handle(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 86)(this);
		}
		material_handle next_material(material_handle h)
		{
			using fn = material_handle(__thiscall*)(void*, material_handle);
			return vmt::get_vfunc<fn>(this, 87)(this, h);
		}
		material_handle invalid_material()
		{
			using fn = material_handle(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 88)(this);
		}
		c_material* get_material(material_handle h)
		{
			using fn = c_material*(__thiscall*)(void*, material_handle);
			return vmt::get_vfunc<fn>(this, 89)(this, h);
		}
	};
} // namespace ap::sdk