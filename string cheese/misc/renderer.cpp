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
	void draw_3d_box(vec3f origin, int w, int h, rgba8 outline)
	{
		float d_w = float(w / 2);
		float d_h = float(h / 2);

		vec3f box_vec3f[8] = {
			vec3f(origin[0] - d_w, origin[1] - d_h, origin[2] - d_w),
			vec3f(origin[0] - d_w, origin[1] - d_h, origin[2] + d_w),
			vec3f(origin[0] + d_w, origin[1] - d_h, origin[2] + d_w),
			vec3f(origin[0] + d_w, origin[1] - d_h, origin[2] - d_w),
			vec3f(origin[0] - d_w, origin[1] + d_h, origin[2] - d_w),
			vec3f(origin[0] - d_w, origin[1] + d_h, origin[2] + d_w),
			vec3f(origin[0] + d_w, origin[1] + d_h, origin[2] + d_w),
			vec3f(origin[0] + d_w, origin[1] + d_h, origin[2] - d_w),
		};

		static vec2i vec0, vec1, vec2, vec3,
			vec4, vec5, vec6, vec7;

		if (world_to_screen(box_vec3f[0], vec0) &&
			world_to_screen(box_vec3f[1], vec1) &&
			world_to_screen(box_vec3f[2], vec2) &&
			world_to_screen(box_vec3f[3], vec3) &&
			world_to_screen(box_vec3f[4], vec4) &&
			world_to_screen(box_vec3f[5], vec5) &&
			world_to_screen(box_vec3f[6], vec6) &&
			world_to_screen(box_vec3f[7], vec7)) {

			vec2i lines[12][2];
			lines[0][0] = vec0;
			lines[0][1] = vec1;
			lines[1][0] = vec1;
			lines[1][1] = vec2;
			lines[2][0] = vec2;
			lines[2][1] = vec3;
			lines[3][0] = vec3;
			lines[3][1] = vec0;
			lines[4][0] = vec4;
			lines[4][1] = vec5;
			lines[5][0] = vec5;
			lines[5][1] = vec6;
			lines[6][0] = vec6;
			lines[6][1] = vec7;
			lines[7][0] = vec7;
			lines[7][1] = vec4;
			lines[8][0] = vec0;
			lines[8][1] = vec4;
			lines[9][0] = vec1;
			lines[9][1] = vec5;
			lines[10][0] = vec2;
			lines[10][1] = vec6;
			lines[11][0] = vec3;
			lines[11][1] = vec7;

			for (int i = 0; i < 12; i++) {
				render_line(vec2i(lines[i][0][0], lines[i][0][1]), vec2i(lines[i][1][0], lines[i][1][1]), outline);
			}
		}
	}
} // namespace ap::renderer