#include "menu.h"
#include <sstream>
#include "toenail/style.h"
namespace ap::text_menu {
	menu::menu(void) {
		m_elements.push_back(new boolea(L"box_esp", L"esp_esp_boxes"));
		m_elements.push_back(new boolea(L"corner_box_esp", L"esp_esp_corner_boxes"));
		m_elements.push_back(new boolea(L"name_esp",  L"esp_name_esp"));
		m_elements.push_back(new boolea(L"health_esp", L"esp_health_boxes"));
		m_elements.push_back(new boolea(L"backtrack_stick_figure_esp", L"esp_backtrack_stick_figure"));
		m_elements.push_back(new boolea(L"dont_render_team_esp", L"esp_dont_render_team"));
		m_elements.push_back(new boolea(L"armour_flags_esp", L"esp_armour_flags"));
		m_elements.push_back(new boolea(L"visualize_backtrack_dots_esp", L"esp_visualize_backtrack_dots"));
		m_elements.push_back(new boolea(L"visualize_backtrack_octagon_esp", L"esp_visualize_backtrack_octagon"));
		m_elements.push_back(new boolea(L"no_scope_lines_esp", L"esp_no_scope_lines"));
		m_elements.push_back(new boolea(L"world_modulation_esp", L"esp_world_modulation"));
		m_elements.push_back(new boolea(L"snap_lines_esp", L"esp_snap_lines"));
		m_elements.push_back(new boolea(L"remove_smoke_esp", L"esp_remove_smoke"));
		m_elements.push_back(new boolea(L"no_flash_esp", L"esp_no_flash"));
		m_elements.push_back(new boolea(L"force_crosshair_esp", L"esp_force_crosshair"));
		m_elements.push_back(new boolea(L"custom_crosshair_esp", L"esp_custom_crosshair"));
		m_elements.push_back(new boolea(L"no_scope_esp", L"esp_no_scope"));
		m_elements.push_back(new boolea(L"hand_chams_esp", L"esp_hand_chams"));
		m_elements.push_back(new boolea(L"enemy_chams_esp", L"esp_enemy_chams"));
		m_elements.push_back(new boolea(L"weapon_chams_esp", L"esp_weapon_chams"));
		m_elements.push_back(new boolea(L"legit_backtrack_aim", L"aim_legit_backtrack"));
		m_elements.push_back(new boolea(L"no_recoil_aim", L"aim_no_recoil"));
		m_elements.push_back(new boolea(L"auto_jump_misc", L"misc_auto_jump"));
		m_elements.push_back(new boolea(L"legit_auto_strafer_misc", L"misc_legit_auto_strafer"));
		m_elements.push_back(new boolea(L"no_crouch_cooldown_misc", L"misc_no_crouch_cooldown"));
		m_elements.push_back(new boolea(L"fast_crouch_loop_misc", L"misc_fast_crouch_loop"));
		m_elements.push_back(new boolea(L"radar_set_spotted_misc", L"misc_radar_set_spotted"));
		m_elements.push_back(new boolea(L"clan_tag_changer_misc", L"misc_clan_tag_changer"));
		m_elements.push_back(new boolea(L"post_processing_misc", L"misc_post_processing"));
		m_elements.push_back(new boolea(L"desync_freestanding_aa", L"aa_desync_freestanding"));
		m_elements.push_back(new boolea(L"emotion_pitch_aa", L"aa_emotion_pitch"));
		m_elements.push_back(new boolea(L"slide_walk_aa", L"aa_slide_walk"));
		


		//m_elements.push_back(new float_slider(L"example_float", L"flt1", 0.f, 1.1f, 0.1f, L"f"));
		//m_elements.push_back(new separator(L"esp"));
		//m_elements.push_back(new combo(L"example_combo", L"cmb", { L"opt1", L"opt2", L"opt3" }));

		this->load();
	}

	float menu::_get(std::wstring var)
	{
		for (auto& e : m_elements)
		{
			if (e->m_variable.compare(var) != 0) {
				continue;
			}
			else {
				return e->m_value;
			}
		}

		return 0.0f;
	}

	void menu::draw_bool(boolea* draw)
	{
		auto index = 0;

		for (; index < m_elements.size(); ++index)
		{
			if (m_elements[index] != (element*)draw)
			{
				continue;
			}
			else
			{
				break;
			}
		}

		m_pos = vec2i(30, 15 * (index + 1));

		auto enabled = !!_wtoi(std::to_wstring(draw->m_value).c_str());
		auto selected = (m_selected == index);
		auto font = toenail::window_title_font;

		auto color_selected = rgba8(0, 175, 255);
		auto color_enabled = rgba8::GREEN();
		auto color_disabled = rgba8::RED();
		auto color_white = rgba8::WHITE();;

		renderer::render_filled_rect(vec2i(m_pos[0], m_pos[1]), vec2i(m_pos[0] + 220, 15), selected ? rgba8(35, 35, 35) : rgba8(40, 40, 40));
		renderer::render_text(vec2i(m_pos[0], m_pos[1]), selected ? color_selected : color_white, font, draw->m_name.c_str(), false);
		renderer::render_text(vec2i(m_pos[0] + 200, m_pos[1]), enabled ? color_enabled : color_disabled, font, enabled ? L"enabled" : L"disabled", false);
	}

	void menu::draw_float(float_slider * draw)
	{
		auto index = 0;

		for (; index < m_elements.size(); ++index)
		{
			if (m_elements[index] != (element*)draw) {
				continue;
			}
			else {
				break;
			}
		}

		auto _round = [](float var) -> float {
			float value = (int)(var * 100.f + .5);
			return (float)value / 100.f;
		};

		m_pos = vec2i(30, 15 * (index + 1));

		auto enabled = (bool)(draw->m_value >= 0.1f);
		auto selected = (m_selected == index);
		auto font = toenail::window_title_font;

		auto color_selected = rgba8(0, 175, 255);
		auto color_enabled = rgba8::GREEN();
		auto color_disabled = rgba8::RED();
		auto color_white = rgba8::WHITE();;

		std::wstringstream wss;
		wss << std::fixed;
		wss.precision(2); // set # places after decimal
		wss << draw->m_value << draw->m_additive;

		renderer::render_filled_rect(vec2i(m_pos[0], m_pos[1]), vec2i(m_pos[0] + 220, 30), selected ? rgba8(35, 35, 35) : rgba8(40, 40, 40));
		renderer::render_text(vec2i(m_pos[0], m_pos[1]), selected ? color_selected : color_white, font, draw->m_name.c_str(), false);
		renderer::render_text(vec2i(m_pos[0] + 200, m_pos[1]), enabled ? color_enabled : color_disabled, font, wss.str(), false);
	}

	void menu::draw_int(int_slider * draw)
	{
		auto index = 0;

		for (; index < m_elements.size(); ++index)
		{
			if (m_elements[index] != (element*)draw)
			{
				continue;
			}
			else
			{
				break;
			}
		}

		m_pos = vec2i(30, 15 * (index + 1));

		auto enabled = !!atoi(std::to_string(draw->m_value).c_str());
		auto selected = (m_selected == index);
		auto font = toenail::window_title_font;

		auto color_selected = rgba8(0, 175, 255);
		auto color_enabled = rgba8::GREEN();
		auto color_disabled = rgba8::RED();
		auto color_white = rgba8::WHITE();;

		auto value = std::wstring(std::to_wstring((int)draw->m_value));
		value.append(draw->m_additive);

		renderer::render_filled_rect(vec2i(m_pos[0], m_pos[1]), vec2i(m_pos[0] + 220, 15), selected ? rgba8(35, 35, 35) : rgba8(40, 40, 40));
		renderer::render_text(vec2i(m_pos[0], m_pos[1]), selected ? color_selected : color_white, font, draw->m_name.c_str(), false);
		renderer::render_text(vec2i(m_pos[0] + 200, m_pos[1]), enabled ? color_enabled : color_disabled, font, value.c_str(), false);
	}

	void menu::draw_combo(combo * draw)
	{
		auto index = 0;

		for (; index < m_elements.size(); ++index)
		{
			if (m_elements[index] != (element*)draw)
			{
				continue;
			}
			else
			{
				break;
			}
		}

		m_pos = vec2i(30, 15 * (index + 1));

		auto enabled = !!atoi(std::to_string(draw->m_value).c_str());
		auto selected = (m_selected == index);
		auto font = toenail::window_title_font;

		auto color_selected = rgba8(0, 175, 255);
		auto color_enabled = rgba8::GREEN();
		auto color_disabled = rgba8::RED();
		auto color_white = rgba8::WHITE();;

		auto value = draw->m_container[(int)draw->m_value];

		renderer::render_filled_rect(vec2i(m_pos[0], m_pos[1]), vec2i(m_pos[0] + 220, 15), selected ? rgba8(35, 35, 35) : rgba8(40, 40, 40));
		renderer::render_text(vec2i(m_pos[0], m_pos[1]), selected ? color_selected : color_white, font, draw->m_name.c_str(), false);
		renderer::render_text(vec2i(m_pos[0] + 200, m_pos[1]), enabled ? color_enabled : color_disabled, font, value.c_str(), false);
	}

	void menu::draw_separator(separator * draw)
	{
		auto index = 0;

		for (; index < m_elements.size(); ++index)
		{
			if (m_elements[index] != (element*)draw)
			{
				continue;
			}
			else
			{
				break;
			}
		}

		m_pos = vec2i(30, 15 * (index + 1));

		auto font = toenail::window_title_font;
		auto selected = (m_selected != index);
		auto color_white = rgba8::WHITE();;

		renderer::render_filled_rect(vec2i(m_pos[0], m_pos[1]), vec2i(m_pos[0] + 220, 30), rgba8(40, 40, 40));
		renderer::render_text(vec2i(m_pos[0], m_pos[1]), selected ? color_white : rgba8(0, 175, 255), font, draw->m_name.c_str(), false);
	}

	// ugliest fn ever ;3
	void menu::handle_input(void)
	{
		auto get_key = [&](int key)
		{
			static bool was_pressed[256];
			auto pressed = GetAsyncKeyState(key);

			if (!was_pressed[key] && pressed)
			{
				was_pressed[key] = true;
			}
			else if (was_pressed[key] && !pressed)
			{
				was_pressed[key] = false;
				return true;
			}

			return false;
		};


		if (get_key(VK_UP))
		{
			m_selected--;
		}

		else if (get_key(VK_DOWN))
		{
			m_selected++;
		}

		if (m_selected > m_elements.size() - 1) m_selected = 0;
		else if (m_selected < 0) m_selected = m_elements.size() - 1;

		auto element = m_elements[m_selected];

		if (element->m_type == element_type::boolea) {
			auto element_casted = (boolea*)element;

			if (get_key(VK_LEFT)) {
				element_casted->m_value = 0;
			}
			else if (get_key(VK_RIGHT)) {
				element_casted->m_value = 1;
			}
		    if (element_casted->m_value == 0 && get_key(VK_RETURN)) {
				element_casted->m_value = 1;
			}
			else if (element_casted->m_value == 1 && get_key(VK_RETURN)) {
				element_casted->m_value = 0;
			}
		}

		else if (element->m_type == element_type::float_slider) {
			auto element_casted = (float_slider*)element;

			if (get_key(VK_LEFT)) {
				auto predicted = element_casted->m_value - element_casted->m_increment;

				if (predicted >= element_casted->m_min) {
					element_casted->m_value -= element_casted->m_increment;
				}
			}
			else if (get_key(VK_RIGHT)) {
				auto predicted = element_casted->m_value + element_casted->m_increment;

				if (predicted <= element_casted->m_max) {
					element_casted->m_value += element_casted->m_increment;
				}
			}
		}

		else if (element->m_type == element_type::int_slider) {
			auto element_casted = (int_slider*)element;

			if (get_key(VK_LEFT)) {
				auto predicted = element_casted->m_value - 1;

				if (predicted >= element_casted->m_min)
				{
					element_casted->m_value--;
				}
			}
			else if (get_key(VK_RIGHT))
			{
				auto predicted = element_casted->m_value + 1;

				if (predicted <= element_casted->m_max)
				{
					element_casted->m_value++;
				}
			}
		}

		else if (element->m_type == element_type::combo)
		{
			auto element_casted = (combo*)element;

			if (get_key(VK_LEFT))
			{
				auto predicted = element_casted->m_value - 1;

				if (predicted >= 0)
				{
					element_casted->m_value--;
				}
			}
			else if (get_key(VK_RIGHT))
			{
				auto predicted = element_casted->m_value + 1;

				if (predicted <= element_casted->m_container.size() - 1)
				{
					element_casted->m_value++;
				}
			}
		}
	}

	void menu::load(void)
	{
		static WCHAR path[MAX_PATH];
		std::wstring folder, file;
	
		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, path)))
		{
			folder = std::wstring(path) + (L"\\sdk\\");
			file = std::wstring(path) + (L"\\sdk\\config.sdk");
		}
	
		CreateDirectoryW(folder.c_str(), NULL);
	
		for (auto& e : m_elements)
		{
			wchar_t value[32] = { '\0' };
	
			GetPrivateProfileStringW(L"sdk", e->m_name.c_str(), L"", value, 32, file.c_str());
	
			e->m_value = _wtof(value);
		}
	}
	
	void menu::save(void)
	{
		static WCHAR path[MAX_PATH];
		std::wstring folder, file;
	
		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, path)))
		{
			folder = std::wstring(path) + (L"\\sdk\\");
			file = std::wstring(path) + (L"\\sdk\\config.sdk");
		}
	
		CreateDirectoryW(folder.c_str(), NULL);
	
		for (auto& e : m_elements)
		{
			WritePrivateProfileStringW(L"sdk", e->m_name.c_str(), std::to_wstring(e->m_value).c_str(), file.c_str());
		}
	}

	void menu::tick(void)
	{
		auto handle_open = [&](void) {
			static bool was_pressed = false;
			auto pressed = GetAsyncKeyState(VK_INSERT);

			if (!was_pressed && pressed) {
				was_pressed = true;
			}
			else if (was_pressed && !pressed) {
				was_pressed = false;
				return true;
			}

			return false;
		};

		if (handle_open())
		{
			m_enabled = !m_enabled;

			if (m_enabled) {
				//load();
			}
			else {
				//save();
			}
		}

		if (!m_enabled) {
			return;
		}

		handle_input();

		for (auto& e : m_elements) {
			if (e->m_type == element_type::boolea) {
				draw_bool((boolea*)e);
			}
			else if (e->m_type == element_type::float_slider) {
				draw_float((float_slider*)e);
			}
			else if (e->m_type == element_type::int_slider) {
				draw_int((int_slider*)e);
			}
			else if (e->m_type == element_type::combo) {
				draw_combo((combo*)e);
			}
			else if (e->m_type == element_type::seperator) {
				draw_separator((separator*)e);
			}
		}
	}
}