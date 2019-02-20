#include "renderer.h"

#include "interfaces.h"
#include "../misc/utils.h"

#include "../sdk/c_surface.h"
#include "../sdk/c_engine.h"


namespace ap::renderer
{
	bool world_to_screen(const vec3f& world, vec2i& screen)
	{
		const auto& view_matrix = interfaces::engine->get_view_matrix();

		float x2 = view_matrix[0][0] * world[0] + view_matrix[0][1] * world[1] + view_matrix[0][2] * world[2] + view_matrix[0][3];
		float y2 = view_matrix[1][0] * world[0] + view_matrix[1][1] * world[1] + view_matrix[1][2] * world[2] + view_matrix[1][3];
		float w = view_matrix[3][0] * world[0] + view_matrix[3][1] * world[1] + view_matrix[3][2] * world[2] + view_matrix[3][3];

		if (w < 0.0001f)
			return false;

		vec2i screen_size;
		interfaces::engine->get_screen_size(screen_size);

		float invw = 1.f / w;
		x2 *= invw;
		y2 *= invw;

		float x = screen_size[0] / 2.f;
		float y = screen_size[1] / 2.f;
		x += 0.5f * x2 * screen_size[0] + 0.5f;
		y -= 0.5f * y2 * screen_size[1] + 0.5f;
		screen[0] = int(x);
		screen[1] = int(y);

		return true;
	}

	unsigned int create_font(const std::string& font_name, int size, int weight, int blur, int scanlines, int flags)
	{
		unsigned long font = interfaces::surface->create_font();
		interfaces::surface->set_font_glyph_set(font, font_name.c_str(), size, weight, blur, scanlines, flags);
		return font;
	}
	vec2i get_text_size(unsigned int font, const std::wstring& text)
	{
		vec2i size;
		interfaces::surface->get_text_size(font, text.c_str(), size[0], size[1]);
		return size;
	}

	void render_text(vec2i position, rgba8 color, unsigned int font, const std::wstring& text, bool center_width, bool center_height)
	{
		const auto text_size = get_text_size(font, text);
		if (center_width)
			position[0] -= text_size[0] / 2;
		if (center_height)
			position[1] -= text_size[1] / 2;

		interfaces::surface->set_text_color(color);
		interfaces::surface->set_text_font(font);
		interfaces::surface->set_text_pos(position);
		interfaces::surface->draw_text(text.c_str(), text.size());
	}
	void render_text(vec2i position, unsigned int font, const std::wstring& text, bool center_width, bool center_height, rgba8 color)
	{
		const auto text_size = get_text_size(font, text);
		if (center_width)
			position[0] -= text_size[0] / 2;
		if (center_height)
			position[1] -= text_size[1] / 2;

		interfaces::surface->set_text_color(color);
		interfaces::surface->set_text_font(font);
		interfaces::surface->set_text_pos(position);
		interfaces::surface->draw_text(text.c_str(), text.size());
	}
	void render_line(vec2i pos1, vec2i pos2, rgba8 color)
	{
		interfaces::surface->set_draw_color(color);
		interfaces::surface->draw_line(pos1, pos2);
	}
	void render_filled_rect(vec2i pos1, vec2i pos2, rgba8 color)
	{
		interfaces::surface->set_draw_color(color);
		interfaces::surface->draw_filled_rect(pos1[0], pos1[1], pos2[0], pos2[1]);
	}
	void render_empty_rect(vec2i pos1, vec2i pos2, rgba8 color)
	{
		interfaces::surface->set_draw_color(color);
		interfaces::surface->draw_empty_rect(pos1[0], pos1[1], pos2[0], pos2[1]);
	}
	void render_filled_polygon(const std::vector<vec4f>& vertices, rgba8 color)
	{
		static int texture_id = interfaces::surface->create_new_texture(true);
		if (static bool is_init = false; !is_init)
		{
			is_init = true; rgba8 c = rgba8::WHITE();
			interfaces::surface->set_texture_rgba(texture_id, &c, 1, 1);
		}

		interfaces::surface->set_draw_color(color);
		interfaces::surface->set_texture(texture_id);
		interfaces::surface->draw_textured_polygon(vertices.size(), vertices.data(), true);
	}
	void render_filled_circle(int x, int y, int radius, int segments, rgba8 color, float offset)
	{
		std::vector<vec4f> vertices;

		float step = pi_f * 2.f / float(segments);
		for (float a = offset; a <= (pi_f * 2.f) + offset; a += step)
			vertices.push_back(vec4f(radius * std::cos(a) + x, radius * std::sin(a) + y, 1.f, 0.f));

		render_filled_polygon(vertices, color);
	}
	void draw_corner_box(int X, int Y, int W, int H, rgba8 Color) {
		int lineW = (W / 5);
		int lineH = (H / 6);
		int lineT = 1;

		//outline
		ap::interfaces::surface->set_draw_color(rgba8(0, 0, 0, 255));
		ap::interfaces::surface->draw_line(vec2i(X - lineT, Y - lineT), vec2i(X + lineW, Y - lineT));
		ap::interfaces::surface->draw_line(vec2i(X - lineT, Y - lineT), vec2i(X - lineT, Y + lineH));
		ap::interfaces::surface->draw_line(vec2i(X - lineT, Y + H - lineH), vec2i(X - lineT, Y + H + lineT)); //bot left
		ap::interfaces::surface->draw_line(vec2i(X - lineT, Y + H + lineT), vec2i(X + lineW, Y + H + lineT));
		ap::interfaces::surface->draw_line(vec2i(X + W - lineW, Y - lineT), vec2i(X + W + lineT, Y - lineT)); // top right
		ap::interfaces::surface->draw_line(vec2i(X + W + lineT, Y - lineT), vec2i(X + W + lineT, Y + lineH));
		ap::interfaces::surface->draw_line(vec2i(X + W + lineT, Y + H - lineH), vec2i(X + W + lineT, Y + H + lineT)); // bot right
		ap::interfaces::surface->draw_line(vec2i(X + W - lineW, Y + H + lineT), vec2i(X + W + lineT, Y + H + lineT));

		//inline
		ap::interfaces::surface->set_draw_color(Color);
		ap::interfaces::surface->draw_line(vec2i(X, Y), vec2i(X, Y + lineH));//top left
		ap::interfaces::surface->draw_line(vec2i(X, Y), vec2i(X + lineW, Y));
		ap::interfaces::surface->draw_line(vec2i(X + W - lineW, Y), vec2i(X + W, Y)); //top right
		ap::interfaces::surface->draw_line(vec2i(X + W, Y), vec2i(X + W, Y + lineH));
		ap::interfaces::surface->draw_line(vec2i(X, Y + H - lineH), vec2i(X, Y + H)); //bot left
		ap::interfaces::surface->draw_line(vec2i(X, Y + H), vec2i(X + lineW, Y + H));
		ap::interfaces::surface->draw_line(vec2i(X + W - lineW, Y + H), vec2i(X + W, Y + H));//bot right
		ap::interfaces::surface->draw_line(vec2i(X + W, Y + H - lineH), vec2i(X + W, Y + H));
	}
	void render_empty_circle(int x, int y, int radius, int segments, rgba8 color)
	{
		interfaces::surface->set_draw_color(color);
		interfaces::surface->draw_circle(x, y, radius, segments);
	}
} // namespace ap::renderer