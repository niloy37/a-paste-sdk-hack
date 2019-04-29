#pragma once

#include "vector.h"
#include "color.h"

#include <string>
#include <vector>


namespace ap::renderer
{
	enum fontflag
	{
		none,
		italic = 0x001,
		underline = 0x002,
		strikeout = 0x004,
		symbol = 0x008,
		antialias = 0x010,
		gaussianblur = 0x020,
		rotary = 0x040,
		dropshadow = 0x080,
		additive = 0x100,
		outline = 0x200,
		custom = 0x400,
		bitmap = 0x800,
	};

	bool world_to_screen(const vec3f& world, vec2i& screen);

	unsigned int create_font(const std::string& font_name, int size, int weight, int blur, int scanlines, int flags = fontflag::none);
	vec2i get_text_size(unsigned int font, const std::wstring& text);

	void render_text(vec2i position, rgba8 color, unsigned int font, const std::wstring& text, bool center_width = false, bool center_height = false);
	void render_line(vec2i pos1, vec2i pos2, rgba8 color);
	void render_filled_rect(vec2i pos1, vec2i pos2, rgba8 color);
	void render_empty_rect(vec2i pos1, vec2i pos2, rgba8 color);
	void render_filled_polygon(const std::vector<vec4f>& vertices, rgba8 color);
	void render_filled_circle(int x, int y, int radius, int segments, rgba8 color, float offset = 0.f);
	void render_empty_circle(int x, int y, int radius, int segments, rgba8 color);
	void draw_corner_box(int X, int Y, int W, int H, rgba8 Color);
	void draw_3d_box(vec3f origin, int w, int h, rgba8 outline);
} // namespace ap::renderer