#pragma once

#include "../misc/vmt.h"
#include "../misc/vector.h"
#include "../misc/vector.h"


namespace ap::sdk
{
	class c_material;

	struct c_model_render_info
	{
		c_model_render_info()
		{
			pModelToWorld = NULL;
			pLightingOffset = NULL;
			pLightingOrigin = NULL;
		}

		vec3f origin; /// 0x0
		vec3f angles; /// 0xC
		char pad[0x4]; /// 0x18
		void *pRenderable; /// 0x1C
		const void *pModel; /// 0x20
		const matrix3x4_t *pModelToWorld; /// 0x24
		const matrix3x4_t *pLightingOffset; /// 0x28
		const vec3f *pLightingOrigin; /// 0x2C
		int flags; /// 0x30
		int entity_index; /// 0x34
		int skin;
		int body;
		int hitboxset;
		unsigned short instance;
	};

	class c_model_render
	{
	public:
		void forced_material_override(c_material* material)
		{
			using fn = void(__thiscall*)(void*, c_material*, int, int);
			vmt::get_vfunc<fn>(this, 1)(this, material, 0, NULL);
		}

	private:
	};
} // namespace ap::sdk