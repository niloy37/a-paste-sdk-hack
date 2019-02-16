#pragma once
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"
#include "../sdk/c_cvar.h"
#include "../sdk/materials.h"

namespace ap::features::nightmode
{
	void modulate_world()
	{
		
		static rgbaf last_world_color,
			last_prop_color,
			last_skybox_color;
		static constexpr rgbaf world_color(0.025f, 0.025f, 0.025f, 0.025f),
			prop_color(0.2f, 0.f, 0.2f, 0.4f),
			skybox_color(0.6f, 0.2f, 0.4f);
	
		static auto r_DrawSpecificStaticProp = ap::interfaces::cvar->find_var("r_DrawSpecificStaticProp");
		if (r_DrawSpecificStaticProp->get_int() != 0)
			r_DrawSpecificStaticProp->set_value("0");
	
		bool did_color_change = (world_color != last_world_color) ||
			(prop_color != last_prop_color) ||
			(skybox_color != last_skybox_color);
	
		static bool was_in_game = false;
		if (!was_in_game && ap::interfaces::engine->is_in_game())
			did_color_change = true;
		was_in_game = ap::interfaces::engine->is_in_game();
	
		// fuck ur dad twice over
		/* hey bolbi dont fuck my dad thx */
		if (!did_color_change)
			return;
	
		for (auto i = ap::interfaces::material_system->first_material(); i != ap::interfaces::material_system->invalid_material(); i = ap::interfaces::material_system->next_material(i))
		{
			auto material = ap::interfaces::material_system->get_material(i);
			if (!material)
				continue;
	
			if (strstr(material->get_texture_group_name(), "World"))
				material->color_modulate(world_color);
			else if (strstr(material->get_texture_group_name(), "StaticProp"))
			{
				material->color_modulate(prop_color);
				material->alpha_modulate(prop_color[3]);
			}
			else if (strstr(material->get_texture_group_name(), "SkyBox"))
				material->color_modulate(skybox_color);
		}
	
		last_world_color = world_color;
		last_prop_color = prop_color;
		last_skybox_color = skybox_color;
	}

	void on_framestage_notify()
	{
		modulate_world();
	}

}