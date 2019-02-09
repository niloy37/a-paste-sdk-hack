#pragma once

#include "../misc/vmt.h"

#include "../misc/vector.h"
#include "../misc/color.h"


namespace ap::sdk
{
	class c_surface
	{
	public:
		void set_draw_color(rgba8 color)
		{
			using fn = void(__thiscall*)(void*, rgba8);
			vmt::get_vfunc<fn>(this, 14)(this, color);
		}
		void draw_line(vec2i pos1, vec2i pos2)
		{
			using fn = void(__thiscall*)(void*, vec2i, vec2i);
			vmt::get_vfunc<fn>(this, 19)(this, pos1, pos2);
		}
		void draw_text(const wchar_t* text, int len, int draw_type = 0)
		{
			using fn = void(__thiscall*)(void*, wchar_t const*, int, int);
			vmt::get_vfunc<fn>(this, 28)(this, text, len, draw_type);
		}
		void set_text_pos(vec2i position)
		{
			using fn = void(__thiscall*)(void*, vec2i);
			vmt::get_vfunc<fn>(this, 26)(this, position);
		}
		void set_text_font(unsigned int font)
		{
			using fn = void(__thiscall*)(void*, unsigned int);
			vmt::get_vfunc<fn>(this, 23)(this, font);
		}
		void set_text_color(rgba8 color)
		{
			using fn = void(__thiscall*)(void*, int, int, int, int);
			vmt::get_vfunc<fn>(this, 25)(this, int(color[0]), int(color[1]), int(color[2]), int(color[3]));
		}
		unsigned int create_font()
		{
			using fn = unsigned int(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 71)(this);
		}
		void set_font_glyph_set(unsigned long& font, const char* font_name, int tall, int weight, int blur, int scanlines, int flags)
		{
			using fn = void(__thiscall*)(void*, unsigned long, const char*, int, int, int, int, int, int, int);
			vmt::get_vfunc<fn>(this, 72)(this, font, font_name, tall, weight, blur, scanlines, flags, 0, 0);
		}
		bool get_text_size(int font, const wchar_t* text, int& width, int& height)
		{
			using fn = bool(__thiscall*)(void*, int, const wchar_t*, int&, int&);
			return vmt::get_vfunc<fn>(this, 79)(this, font, text, width, height);
		}
		void draw_filled_rect(int x, int y, int x2, int y2)
		{
			using fn = void(__thiscall*)(void*, int, int, int, int);
			return vmt::get_vfunc<fn>(this, 16)(this, x, y, x2, y2);
		}
		void draw_empty_rect(int x, int y, int x2, int y2)
		{
			using fn = void(__thiscall*)(void*, int, int, int, int);
			return vmt::get_vfunc<fn>(this, 18)(this, x, y, x2, y2);
		}
		int create_new_texture(bool procedural = false)
		{
			using fn = int(__thiscall*)(void*, bool);
			return vmt::get_vfunc<fn>(this, 43)(this, procedural);
		}
		void draw_circle(int x, int y, int radius, int segments)
		{
			using fn = void(__thiscall*)(void*, int, int, int, int);
			return vmt::get_vfunc<fn>(this, 103)(this, x, y, radius, segments);
		}
		void draw_textured_polygon(int count, const vec4f* vertices, bool clip = true)
		{
			using fn = void(__thiscall*)(void*, int, const vec4f*, bool);
			return vmt::get_vfunc<fn>(this, 106)(this, count, vertices, clip);
		}
		void set_texture(int id)
		{
			using fn = void(__thiscall*)(void*, int);
			return vmt::get_vfunc<fn>(this, 38)(this, id);
		}
		void set_texture_rgba(int id, const rgba8* rgba, int wide, int tall)
		{
			using fn = void(__thiscall*)(void*, int, const rgba8*, int, int);
			return vmt::get_vfunc<fn>(this, 37)(this, id, rgba, wide, tall);
		}

	private:
	};
} // namespace ap::sdk