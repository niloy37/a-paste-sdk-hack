#pragma once
// includes 
#include <list>
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"
#include "../sdk/c_client_state.h"
#include "../sdk/c_globals.h"
#include "../sdk/c_player_info.h"
#include "../misc/renderer.h"
#include "../features/backtrack.h"
#include "../sdk/materials.h"
#include "../sdk/c_surface.h"

namespace ap::features::visuals {

	struct box_data {
		int x, y, w, h;
	};

	void health_bar(ap::sdk::c_base_entity * pEntity, box_data size) {
		// credits to that edgy healthbars post on UC
		box_data HealthBar = size;
		HealthBar.y += (HealthBar.h + 6);
		HealthBar.h = 4;

		int HealthValue = int(pEntity->get_health());
		if (HealthValue > 100) {
			HealthValue = 100;
		}

		int HealthPerc = int(HealthValue) / 100;
		int Width = (size.w * HealthPerc);
		HealthBar.w = Width;

		// --  Main Bar -- //
		int flBoxes = pEntity->get_health() / 10;
		int flX = size.x - 7;
		int flY = size.y - 1;
		int flHeight = size.h / 10;
		int flHeight2 = size.h / 10;
		int flMultiplier = 12 / 360; flMultiplier *= flBoxes - 1;
		//rgba8 ColHealth = rgba8::FromHSB(flMultiplier, 1, 1);
		rgba8 ColHealth = rgba8::GREEN();
		int rectHeight = flHeight * flBoxes + 1;

		renderer::render_filled_rect(vec2i(flX, flY), vec2i(4, size.h + 2) + vec2i(flX, flY), rgba8(80, 80, 80, 255));
		renderer::render_empty_rect(vec2i(flX, flY), vec2i(4, size.h + 2) + vec2i(flX, flY), rgba8(0, 0, 0, 255));

		renderer::render_filled_rect(vec2i(flX + 1, flY + size.h + (flMultiplier - rectHeight) + 1), vec2i(2, rectHeight) + vec2i(flX + 1, flY + size.h + (flMultiplier - rectHeight) + 1), pEntity->IsDormant() ? rgba8(0, 0, 0, 140) : ColHealth);

		for (int i = 0; i < 10; i++) {
			renderer::render_line(vec2i(flX, flY + i * flHeight2), vec2i(flX + 4, flY + i * flHeight2) /*+ vec2i(flX, flY + i * flHeight2)*/, rgba8::BLACK());
			//renderer::render_line(vec2i((size.x - 6), size.y + i * (flHeight2 / 10) - 1), vec2i(size.x - 3, size.y + i * (flHeight2 / 10)), rgba8::BLACK());
		}

	}

	void render_health(ap::sdk::c_base_entity * entity, int x, int y, int h) {
		int health = entity->get_health();

		if (health > 100) {
			health = 100;
		}

		renderer::render_filled_rect((vec2i(x - 6, y)), (vec2i(4, h)) + (vec2i(x - 6, y)), rgba8(0, 0, 0, 125));
		renderer::render_filled_rect((vec2i(x - 6, y)), (vec2i(4, (h / 100) * health)) + (vec2i(x - 6, y)), rgba8(0, 185, 0, 255));
		renderer::render_empty_rect((vec2i(x - 6, y)), (vec2i(4, h)) + (vec2i(x - 6, y)), rgba8(0, 0, 0, 255));
		for (int i = 0; i < 10; i++) {
			//renderer::render_line(vec2i(x, y + i * h), vec2i(x + 4, y + i * h) /*+ vec2i(flX, flY + i * flHeight2)*/, rgba8::BLACK());
			//renderer::render_line(vec2i((size.x - 6), size.y + i * (flHeight2 / 10) - 1), vec2i(size.x - 3, size.y + i * (flHeight2 / 10)), rgba8::BLACK());
		}
	}

	void snap_lines(box_data size) {
		box_data box = size;

		vec2i screen_size;
		interfaces::engine->get_screen_size(screen_size);

		renderer::render_line(vec2i(screen_size[0] / 2, screen_size[1]), vec2i(box.x + box.w, box.y + box.h), rgba8::WHITE());
	}

	void initialize() {
		/* pointer to the local player */
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		/* anti-screenshot */
		if (ap::interfaces::engine->is_taking_screenshot())
			return;

		rgba8 DORMANT_COLOUR = rgba8(128, 128, 128, 255);
		rgba8 TEAM_COLOUR = rgba8::BLUE();
		rgba8 ENEMY_COLOUR = rgba8::RED();

		auto calculate_box_static = [](ap::sdk::c_base_entity * entity, box_data& box) -> bool
		{
			vec3f top, down, air;

			vec2i s[2];

			vec3f adjust = vec3f(0.f, 0.f, -15.f) * entity->get_crouch_amount();

			if (!(entity->get_flags() & FL_ONGROUND) && (entity->get_move_type() != MOVETYPE_LADDER))
				air = vec3f(0.f, 0.f, 10.f);
			else
				air = vec3f(0.f, 0.f, 0.f);

			down = entity->get_abs_origin() + air;
			top = down + vec3f(0.f, 0.f, 72.f) + adjust;

			if (renderer::world_to_screen(top, s[1]) && renderer::world_to_screen(down, s[0]))
			{
				vec2i delta = s[1] - s[0];

				box.h = int(fabsf(float(delta[1]))) + 6;
				box.w = box.h / 2 + 5;

				box.x = s[1][0] - (box.w / 2) + 2;
				box.y = s[1][1] - 1;

				return true;
			}

			return false;
		};

		for (int i = 1; i < interfaces::globals->maxclients; i++) {
			box_data mango_box;
			/* pointer to the entity (clients) */
			ap::sdk::c_base_entity* mango_entity = ap::interfaces::client_entity_list->get_client_entity(i);

			/* if the entity is null, don't do anything */
			if (!mango_entity)
				continue;

			/* if the local player is null, don't do anything */
			if (!mango_local)
				continue;

			/* if the entity is the local player, don't do anything either */
			if (mango_entity == mango_local)
				continue;

			/* if the entity is dead, don't do anything */
			if (!mango_entity->is_alive())
				continue;

			if (mango_entity->is_dormant())
				continue;

			//ap::sdk::c_player_info player_info;

			//ap::interfaces::engine->get_player_info(i, &player_info);

			calculate_box_static(mango_entity, mango_box);
			//calculate_dynamic_box(mango_entity, mango_box);

			//ap::features::visuals::render_health(mango_entity, mango_box.x, mango_box.y, mango_box.h);
			if (mango_local->get_team_num() != mango_entity->get_team_num()) {
				renderer::draw_corner_box(mango_box.x, mango_box.y, mango_box.w, mango_box.h, ENEMY_COLOUR);
				ap::features::visuals::health_bar(mango_entity, mango_box);
				ap::features::visuals::snap_lines(mango_box);
			}
			//else if (mango_local->get_team_num() == mango_entity->get_team_num()){
			//	renderer::draw_corner_box(mango_box.x, mango_box.y, mango_box.w, mango_box.h, TEAM_COLOUR);
			//}
		}
	}

	void remove_smoke() {
		static auto smoke_count = *reinterpret_cast<uint32_t **>(ap::find_signature("client_panorama.dll", "A3 ? ? ? ? 57 8B CB") + 1);

		static std::vector<const char*> smoke_materials = {
			"effects/overlaysmoke",
			"particle/beam_smoke_01",
			"particle/particle_smokegrenade",
			"particle/particle_smokegrenade1",
			"particle/particle_smokegrenade2",
			"particle/particle_smokegrenade3",
			"particle/particle_smokegrenade_sc",
			"particle/smoke1/smoke1",
			"particle/smoke1/smoke1_ash",
			"particle/smoke1/smoke1_nearcull",
			"particle/smoke1/smoke1_nearcull2",
			"particle/smoke1/smoke1_snow",
			"particle/smokesprites_0001",
			"particle/smokestack",
			"particle/vistasmokev1/vistasmokev1",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust",
			"particle/vistasmokev1/vistasmokev1_fire",
			"particle/vistasmokev1/vistasmokev1_nearcull",
			"particle/vistasmokev1/vistasmokev1_nearcull_fog",
			"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
			"particle/vistasmokev1/vistasmokev4_emods_nocull",
			"particle/vistasmokev1/vistasmokev4_nearcull",
			"particle/vistasmokev1/vistasmokev4_nocull"
		};

		for (auto material_name : smoke_materials) {
			sdk::c_material * material = ap::interfaces::material_system->find_material(material_name, "Other textures");

			material->set_material_var_flag(ap::sdk::MATERIAL_VAR_WIREFRAME, true);
		}
		*(int*)smoke_count = 0;
	}

	void no_flash() {
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		float mango = 0.0f;

		mango_local->flash_duration(mango);
	}

	void force_crosshair() {
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		static auto weapon_debug_spread_show = ap::interfaces::cvar->find_var("weapon_debug_spread_show");

		weapon_debug_spread_show->nFlags &= ~sdk::cvar_flags::fcvar_cheat;
		if (mango_local && mango_local->get_health() > 0)
		{
			weapon_debug_spread_show->set_value(mango_local->is_scoped() ? 0 : 3);
		}
	}

	void no_scope_lines() {
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		vec2i screen_size;
		interfaces::engine->get_screen_size(screen_size);

		if (mango_local->is_scoped()) {
			vec2i center;
			center[0] = screen_size[0] / 2;
			center[1] = screen_size[1] / 2;

			renderer::render_line(vec2i(0, center[1]), vec2i(screen_size[0], center[1]), rgba8::BLACK());
			renderer::render_line(vec2i(center[0], 0), vec2i(center[0], screen_size[1]), rgba8::BLACK());
		}
	}

	void crosshair() {
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		vec2i screen_size;
		interfaces::engine->get_screen_size(screen_size);

		vec2i center;
		center[0] = screen_size[0] / 2;
		center[1] = screen_size[1] / 2;

		renderer::render_line(vec2i(center[0] - 10, center[1] - 0), vec2i(center[0] + 10, center[1] + 0), rgba8::GREEN());
		renderer::render_line(vec2i(center[0] + 0, center[1] - 10), vec2i(center[0] - 0, center[1] + 10), rgba8::GREEN());
		renderer::render_line(vec2i(center[0] - 5, center[1] - 0), vec2i(center[0] + 5, center[1] + 0), rgba8::WHITE());
		renderer::render_line(vec2i(center[0] + 0, center[1] - 5), vec2i(center[0] - 0, center[1] + 5), rgba8::WHITE());
	}

	void on_paint_traverse() {
		initialize();
		force_crosshair();
		no_scope_lines();
		crosshair();
	}

	void on_framestage_notify() {
		remove_smoke();
		no_flash();
	}
}