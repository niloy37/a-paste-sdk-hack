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
#include "../sdk/materials.h"
#include "../sdk/c_surface.h"
#include "../toenail/style.h"
#include "../sdk/c_trace.h"
#include "../menu.h"

namespace ap::features::visuals {
#define nigger true
	struct box_data {
		int x, y, w, h;
	};
	
	auto calculate_box_static = [](ap::sdk::c_base_entity * entity, box_data & box) -> bool
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

	void health_bar_ayyware_gg(ap::sdk::c_base_entity* entity, box_data size) {
		if (!variables::health_boxes)
			return;
		box_data box = size;

		const int h = (size.h);
		const int offset = (h / 4) + 5;
		const int w = h / 64;
		int health = entity->get_health();
		int hp = h - (int)((h * health) / 100); // Percentage

		int Red = 255 - (health * 2);
		int Green = health * 2;
		int Blue = health * 2;

		renderer::render_empty_rect(vec2i((size.x - 6) - 1, size.y - 1), vec2i(3, h + 2) + vec2i((size.x - 6) - 1, size.y - 1), rgba8::BLACK());
		renderer::render_empty_rect(vec2i((size.x - 5) - 1, size.y - 1), vec2i(3, h + 2) + vec2i((size.x - 5) - 1, size.y - 1), rgba8::BLACK());
		renderer::render_empty_rect(vec2i((size.x - 4) - 1, size.y - 1), vec2i(3, h + 2) + vec2i((size.x - 4) - 1, size.y - 1), rgba8::BLACK());

		renderer::render_line(vec2i((size.x - 6), size.y + hp), vec2i((size.x - 6), size.y + h), rgba8(Red, Green, 0, 255));
		renderer::render_line(vec2i((size.x - 4), size.y + hp), vec2i((size.x - 4), size.y + h), rgba8(Red, Green, 0, 255));
		renderer::render_line(vec2i((size.x - 5), size.y + hp), vec2i((size.x - 5), size.y + h), rgba8(Red, Green, 0, 255));
	}

	void name_esp(box_data size, int index) {
		if (!variables::name_esp)
			return;
		box_data box = size;

		ap::sdk::c_player_info player_info; 
		ap::interfaces::engine->get_player_info(index, &player_info);

		std::string player_name = player_info.name;
		std::wstring w_player_name = std::wstring(player_name.begin(), player_name.end());

		ap::renderer::render_text(vec2i(box.x + box.w / 2, box.y - 10), rgba8::WHITE(), toenail::menu_properties::window_title_font, w_player_name, true, false);
	}

	void snap_lines(box_data size) {
		if (!variables::snap_lines)
			return;
		box_data box = size;

		vec2i screen_size;
		interfaces::engine->get_screen_size(screen_size);

		renderer::render_line(vec2i(screen_size[0] / 2, screen_size[1]), vec2i(box.x + box.w / 2, box.y + box.h), rgba8::WHITE());
	}

	void armour_check(ap::sdk::c_base_entity * entity, box_data size) {
		if (!variables::armour_flags)
			return;
		box_data box = size;

		if (entity->get_armor() > 0) {
			if (entity->has_helmet()) {
				ap::renderer::render_text(vec2i(box.x + box.w + 4, box.y), rgba8::RED(), toenail::menu_properties::window_title_font, L"HK", false, false);
			}
			else {
				ap::renderer::render_text(vec2i(box.x + box.w + 4, box.y), rgba8::RED(), toenail::menu_properties::window_title_font, L"K", false, false);
			}
		}
	}

	void enemy_aim_positions(ap::sdk::c_base_entity* entity) {
		if (!variables::enemy_aim_positions)
			return;

		vec3f src_f, dst_f, forward;
		vec2i src_i, dst_i;

		ap::angle_vector(entity->get_eye_angle(), forward);
		src_f = ap::get_hitbox_position(entity, 0);
		dst_f = src_f + (forward * 800);

		ap::sdk::Ray_t t_ray;
		t_ray.Init(src_f, dst_f);
		ap::sdk::CTraceFilter t_filter;
		t_filter.pSkip1 = entity;
		ap::sdk::trace_t t_trace;

		ap::interfaces::trace->trace_ray(t_ray, MASK_SHOT, &t_filter, &t_trace);

		if (!renderer::world_to_screen(src_f, src_i) || !renderer::world_to_screen(t_trace.end, dst_i))
			return;

		renderer::render_line(vec2i(src_i[0], src_i[1]), vec2i(dst_i[0], dst_i[1]), rgba8::WHITE());
		renderer::draw_3d_box(vec3f(t_trace.end[0], t_trace.end[1], t_trace.end[2]), 6, 6, rgba8::RED());
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

		for (int i = 1; i < interfaces::globals->maxclients; i++) {
			box_data mango_box;
			/* pointer to the entity (clients) */
			ap::sdk::c_base_entity* mango_entity = ap::interfaces::client_entity_list->get_client_entity(i);

			if (!mango_entity || !mango_local || mango_entity == mango_local || !mango_entity->is_valid())
				continue;

			ap::sdk::c_player_info player_info; 
			ap::interfaces::engine->get_player_info(i, &player_info);

			vec3f top, down, air;
			vec2i s[2];
			vec3f adjust = vec3f(0.f, 0.f, -15.f) * mango_entity->get_crouch_amount();

			if (!(mango_entity->get_flags() & FL_ONGROUND) && (mango_entity->get_move_type() != MOVETYPE_LADDER))
				air = vec3f(0.f, 0.f, 10.f);
			else
				air = vec3f(0.f, 0.f, 0.f);

			down = mango_entity->get_abs_origin() + air;
			top = down + vec3f(0.f, 0.f, 72.f) + adjust;

			if (renderer::world_to_screen(top, s[1]) && renderer::world_to_screen(down, s[0])) {
				calculate_box_static(mango_entity, mango_box);
				//calculate_dynamic_box(mango_entity, mango_box);

				//ap::features::visuals::render_health(mango_entity, mango_box.x, mango_box.y, mango_box.h);
				if (mango_local->get_team_num() != mango_entity->get_team_num()) {
					if (variables::esp_corner_boxes) {
						renderer::draw_corner_box(mango_box.x, mango_box.y, mango_box.w, mango_box.h, ENEMY_COLOUR);
					}
					if (variables::esp_boxes) {
						renderer::render_empty_rect(vec2i(mango_box.x, mango_box.y), vec2i(mango_box.w, mango_box.h) + vec2i(mango_box.x, mango_box.y), rgba8::RED());
						renderer::render_empty_rect(vec2i(mango_box.x, mango_box.y) - 1, vec2i(mango_box.w, mango_box.h) + vec2i(mango_box.x, mango_box.y) - 1, rgba8::BLACK());
						renderer::render_empty_rect(vec2i(mango_box.x, mango_box.y) + 1, vec2i(mango_box.w, mango_box.h) + vec2i(mango_box.x, mango_box.y) + 1, rgba8::BLACK());
					}
					//ap::features::visuals::edgy_health_bar(mango_entity, mango_box);
					ap::features::visuals::name_esp(mango_box, i);
					ap::features::visuals::snap_lines(mango_box);
					ap::features::visuals::armour_check(mango_entity, mango_box);
					ap::features::visuals::enemy_aim_positions(mango_entity);
					ap::features::visuals::health_bar_ayyware_gg(mango_entity, mango_box);
				}
				//else if (mango_local->get_team_num() == mango_entity->get_team_num()){
				//	renderer::draw_corner_box(mango_box.x, mango_box.y, mango_box.w, mango_box.h, TEAM_COLOUR);
				//}
			}
		}
	}

	void remove_smoke() {
		if (!variables::remove_smoke)
			return;

		static auto smoke_count = *reinterpret_cast<uint32_t **>(ap::find_signature("client_panorama.dll", "A3 ? ? ? ? 57 8B CB") + 1);

		int himmeneyskimmeneyboobityniggerityurmomispasterritywobbity = 0;

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
		*(int*)smoke_count = himmeneyskimmeneyboobityniggerityurmomispasterritywobbity;
	}

	void no_flash() {
		if (!variables::no_flash)
			return;
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		float reee = 0.0f;

		mango_local->flash_duration(reee);
	}

	void force_crosshair() {
		if (!variables::force_crosshair)
			return;
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
		if (!variables::no_scope_lines)
			return;
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;
		if (!(mango_local->is_alive()))
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

	void render_custom_crosshair() {
		if (!variables::custom_crosshair)
			return;
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
		render_custom_crosshair();
	}

	void on_framestage_notify() {
		remove_smoke();
		no_flash();
	}
}