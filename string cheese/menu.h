#pragma once
#pragma once
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <ShlObj_core.h>
#include <functional>
#include "../string cheese/misc/renderer.h"
#include "misc/singleton.h"
namespace ap::text_menu {
	enum class element_type
	{
		boolea,
		float_slider,
		int_slider,
		combo,
		seperator
	};

	class element
	{
	public:
		element_type m_type;
		std::wstring m_name;
		std::wstring m_variable;
		int m_index;
		float m_value;
		int m_alpha;
	};

	class separator : public element
	{
	public:
		separator(void)
		{
			m_type = element_type::seperator;
		}

		~separator(void)
		{

		}

		separator(std::wstring name)
		{
			m_name = name;
			m_type = element_type::seperator;
		}
	};


	class boolea : public element
	{
	public:
		boolea(void)
		{
			m_type = element_type::boolea;
		}

		~boolea(void)
		{

		}

		boolea(std::wstring name, std::wstring variable)
		{
			m_variable = variable;
			m_name = name;
			m_type = element_type::boolea;
		}
	};

	class float_slider : public element
	{
	public:
		float_slider(void)
		{
			m_type = element_type::float_slider;
		}

		~float_slider(void)
		{

		}

		float_slider(std::wstring name, std::wstring variable, float min, float max, float increment, std::wstring additive = L"")
		{
			m_type = element_type::float_slider;
			m_name = name;
			m_variable = variable;
			m_min = min;
			m_max = max;
			m_increment = increment;
			m_additive = additive;
			m_value = 0.0;
		}

		float m_min, m_max, m_increment;
		std::wstring m_additive;
	};

	class int_slider : public element
	{
	public:
		int_slider(void)
		{
			m_type = element_type::int_slider;
		}

		~int_slider(void)
		{

		}

		int_slider(std::wstring name, std::wstring variable, int min, int max, std::wstring additive = L"")
		{
			m_type = element_type::int_slider;
			m_name = name;
			m_variable = variable;
			m_min = min;
			m_max = max;
			m_additive = additive;
		}

		int m_min, m_max;
		std::wstring m_additive;
	};

	class combo : public element
	{
	public:
		combo(void)
		{
			m_type = element_type::combo;
		}

		~combo(void)
		{

		}

		combo(std::wstring name, std::wstring variable, std::vector<std::wstring> container)
		{
			m_type = element_type::combo;
			m_name = name;
			m_variable = variable;
			m_container = container;
		}

		std::vector<std::wstring> m_container;
	};

	class menu : public singleton<menu>
	{
	public:
		menu(void);

		~menu(void)
		{
			m_elements.clear();
		}

		float _get(std::wstring var);
		void tick(void);

		void save(void);
		void load(void);
	private:
		std::vector<element*> m_elements;

		ap::vec2i m_pos;

		void handle_input(void);

		bool m_enabled = false;
		int m_selected;

		void draw_bool(boolea* draw);
		void draw_float(float_slider* draw);
		void draw_int(int_slider* draw);
		void draw_combo(combo* draw);
		void draw_separator(separator* draw);
	};

	template <typename t>
	inline t clamp(t number, t min, t max) {
		if (number < min) {
			return min;
		}
		else if (number > max) {
			return max;
		}
		else {
			return number;
		}
	}
}