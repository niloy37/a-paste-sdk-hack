#include "hooks.h"

#include "../ap_core.h"
#include "interfaces.h"
#include "vmt.h"
#include "utils.h"
#include "defines.h"

#include "../sdk/c_user_cmd.h"
#include "../sdk/c_view_setup.h"
#include "../sdk/c_panel.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_base_entity.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_model_render.h"
#include "../sdk/materials.h"
#include "../sdk/recv_data.h"
#include "../sdk/c_input_system.h"

#include "../toenail/menu.h"
#include "../menu.h"

#include "../features/spammers.h"
#include "../features/Movement.h"
#include "../features/worldmodulation.h"
#include "../features/backtrack.h"
#include "../features/radar.h"
#include "../features/visuals.h"
#include "../features/antiaim.h"
#include "../sdk/c_model_render.h"
#include "../sdk/c_render_view.h"
#include "../misc/globalvars.h"
#include "../features/aimbot.h"
namespace
{
	ap::vmt::c_vmt_hook_manager client_mode_hook_manager;
	ap::vmt::c_vmt_hook_manager client_hook_manager;
	ap::vmt::c_vmt_hook_manager render_view_hook_manager;
	ap::vmt::c_vmt_hook_manager panel_hook_manager;
	ap::vmt::c_vmt_hook_manager model_render_hook_manager;
	ap::vmt::c_vmt_hook_manager surface_hook_manager;

	using create_move_fn = bool(__thiscall*)(void*, float, ap::sdk::c_user_cmd*);
	using frame_stage_notify_fn = void(__thiscall*)(void*, int);
	using scene_end_fn = void(__thiscall*)(void*);
	using override_view_fn = void(__thiscall*)(void*, ap::sdk::c_view_setup*);
	using paint_traverse_fn = void(__thiscall*)(void*, unsigned int, bool, bool);
	using draw_model_execute_fn = void(__thiscall*)(void*, void*, void*, const ap::sdk::c_model_render_info&, ap::matrix3x4_t*);
	using lock_cursor_fn = void(__thiscall*)(void*);
	using do_extra_bone_processing_fn = void(__thiscall*)(void*, void*, ap::sdk::studiohdr_t*, ap::vec3f*, ap::vec3f*, ap::matrix3x4_t*, void*, void*);

	create_move_fn original_create_move;
	frame_stage_notify_fn original_frame_stage_notify;
	scene_end_fn original_scene_end;
	override_view_fn original_override_view;
	paint_traverse_fn original_paint_traverse;
	draw_model_execute_fn original_draw_model_execute;
	lock_cursor_fn        original_lock_cursor;
	do_extra_bone_processing_fn        do_extra_bone_processing;

	bool __fastcall hooked_create_move(void* ecx, void* edx, float frametime, ap::sdk::c_user_cmd* mango_cmd)
	{
		original_create_move(ecx, frametime, mango_cmd);
		if (!mango_cmd->command_number)
			return true;

		uintptr_t* pebp;
		__asm mov pebp, ebp;
		bool *send_packet = (bool*)(*pebp - 0x1C);

		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());

		ap::g::mango_local = mango_local;
		ap::g::mango_cmd = mango_cmd;

		if (ap::interfaces::engine->is_connected() && ap::interfaces::engine->is_in_game())
		{
			ap::features::aimbot::no_recoil();
			ap::features::antiaim::on_create_move(mango_cmd);
			ap::features::movement::on_create_move(mango_cmd, send_packet);
			ap::features::radar::on_create_move();
			ap::features::backtrack::on_create_move(mango_cmd);
			ap::features::spammers::on_create_move();
		}

		return false;
	}
	void __stdcall hooked_lock_cursor()
	{
		// turn off & on input
		ap::interfaces::input_system->enable_input(!ap::menu::menu_open);

		// if we have menu opened
		if (ap::menu::menu_open)
			ap::interfaces::surface->unlock_cursor();
		else
			original_lock_cursor(ap::interfaces::surface);
	}
	void __fastcall hooked_frame_stage_notify(void* ecx, void* edx, int stage)
	{
		original_frame_stage_notify(ecx, stage);

		if (ap::interfaces::engine->is_connected() && ap::interfaces::engine->is_in_game())
		{
			ap::features::nightmode::on_framestage_notify();
			ap::features::visuals::on_framestage_notify();
		}

	}
	void __fastcall hooked_scene_end(void* ecx, void* edx)
	{
		original_scene_end(ecx);
	}
	void __fastcall hooked_override_view(void* ecx, void* edx, ap::sdk::c_view_setup* setup)
	{
		original_override_view(ecx, setup);
	}
	void __fastcall hooked_paint_traverse(void* ecx, void* edx, unsigned int panel, bool mango1, bool mango2)
	{
		const std::string panel_name = ap::interfaces::panel->get_name(panel);

		// render stuff penis
		if (panel_name == "MatSystemTopPanel")
		{
			if (ap::text_menu::menu::get()._get(L"misc_watermark")) {
				const std::wstring& bruh = L"CHUM";
				const std::wstring& pasted_watermark = L"THIS WATERMARK PASTED";
				ap::vec2i himmeney;
				ap::interfaces::engine->get_screen_size(himmeney);
				ap::renderer::render_filled_rect(ap::vec2i(himmeney[0] - 162, 4), ap::vec2i(182 + himmeney[0] - 187, 20), ap::rgba8(177, 0, 0, 180));
				ap::renderer::render_text(ap::vec2i(himmeney[0] - 167 + 7, 6), ap::rgba8(255, 255, 255, 255), toenail::window_title_font, pasted_watermark, false, false);
			}
			//ap::menu::run();

			ap::text_menu::menu::get().tick();
			ap::features::visuals::on_paint_traverse();
			ap::features::backtrack::on_paint_traverse();
		}

		// noscope
		if (ap::text_menu::menu::get()._get(L"esp_no_scope")) {
			if (panel_name == "HudZoom")
				return;
		}

		original_paint_traverse(ecx, panel, mango1, mango2);
	}
	void __fastcall hooked_draw_model_execute(void* ecx, void* edx, void* context, void* state, const ap::sdk::c_model_render_info& render_info, ap::matrix3x4_t* matrix)
	{
		ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
			
		const char* model_name = ap::interfaces::model_info->get_model_name((ap::sdk::model_t*)render_info.pModel);
		ap::sdk::c_material* gold_detail = ap::interfaces::material_system->find_material("models/inventory_items/wildfire_gold/wildfire_gold_detail", nullptr);
		ap::sdk::c_material* crystal_blue = ap::interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_blue", nullptr);
		ap::sdk::c_material* crystal_glass = ap::interfaces::material_system->find_material("models/inventory_items/trophy_majors/gloss", nullptr);
		ap::sdk::c_material* animated_darude = ap::interfaces::material_system->find_material("models/inventory_items/music_kit/darude_01/mp3_detail", nullptr);
		ap::sdk::c_material* gold_text = ap::interfaces::material_system->find_material("models/inventory_items/contributor_map_tokens/contributor_charset_color", nullptr);
		ap::sdk::c_material* bus_down_ghost_flames = ap::interfaces::material_system->find_material("models/inventory_items/dreamhack_trophies/dreamhack_star_blur", nullptr);
		ap::sdk::c_material* ghost_flash = ap::interfaces::material_system->find_material("models/inventory_items/dogtags/dogtags_outline", nullptr);
		ap::sdk::c_material* spooky_ghost = ap::interfaces::material_system->find_material("models/inventory_items/dogtags/dogtags_lightray", nullptr);
		ap::sdk::c_material* ghost_flames = ap::interfaces::material_system->find_material("models/extras/speech_info", nullptr);
		ap::sdk::c_material* halo_add = ap::interfaces::material_system->find_material("materials/dev/halo_add_to_screen", nullptr);

		if (ap::interfaces::engine->is_connected() && ap::interfaces::engine->is_in_game()) {
			spooky_ghost->color_modulate(ap::rgbaf::RED());
			
			animated_darude->color_modulate(ap::rgbaf::RED());
			//spooky_ghost->set_material_var_flag(ap::sdk::MATERIAL_VAR_IGNOREZ, true);
			if (ap::text_menu::menu::get()._get(L"esp_hand_chams")) {
				if (strstr(model_name, "arms")) {
					spooky_ghost->set_material_var_flag(ap::sdk::MATERIAL_VAR_WIREFRAME, true);
					ap::interfaces::render_view->set_blend(1.0f);
					ap::interfaces::model_render->forced_material_override(spooky_ghost);
					original_draw_model_execute(ecx, context, state, render_info, matrix);
					
					ap::interfaces::render_view->set_blend(1.5f);
					ap::interfaces::model_render->forced_material_override(animated_darude);
					original_draw_model_execute(ecx, context, state, render_info, matrix);
					
				}
			}
			if (ap::text_menu::menu::get()._get(L"esp_enemy_chams")) {
				if (strstr(model_name, "player")) {
					crystal_blue->set_material_var_flag(ap::sdk::MATERIAL_VAR_WIREFRAME, false);
					crystal_blue->set_material_var_flag(ap::sdk::MATERIAL_VAR_FLAT, true);
					crystal_blue->set_material_var_flag(ap::sdk::MATERIAL_VAR_IGNOREZ, true);
			
					animated_darude->set_material_var_flag(ap::sdk::MATERIAL_VAR_IGNOREZ, true);
					animated_darude->set_material_var_flag(ap::sdk::MATERIAL_VAR_WIREFRAME, true);

					ap::interfaces::render_view->set_blend(1.0f);
					ap::interfaces::model_render->forced_material_override(crystal_blue);
					original_draw_model_execute(ecx, context, state, render_info, matrix);
			
					ap::interfaces::render_view->set_blend(1.5f);
					ap::interfaces::model_render->forced_material_override(animated_darude);
					original_draw_model_execute(ecx, context, state, render_info, matrix);
					
				}
			}
			if (ap::text_menu::menu::get()._get(L"esp_weapon_chams")) {
				if ((strstr(model_name, "weapon")) && (!(strstr(model_name, "arms"))) && (!(strstr(model_name, "knife")))) {
					ghost_flames->set_material_var_flag(ap::sdk::MATERIAL_VAR_WIREFRAME, false);
					ap::interfaces::render_view->set_blend(2.5f);
					ap::interfaces::model_render->forced_material_override(ghost_flames);
					original_draw_model_execute(ecx, context, state, render_info, matrix);
					
					ap::interfaces::render_view->set_blend(0.5f);
					ap::interfaces::model_render->forced_material_override(animated_darude);
					original_draw_model_execute(ecx, context, state, render_info, matrix);
					
				}
			}
			if (ap::text_menu::menu::get()._get(L"esp_weapon_chams")) {
				if ((strstr(model_name, "knife"))) {
					crystal_blue->set_material_var_flag(ap::sdk::MATERIAL_VAR_WIREFRAME, true);
					ap::interfaces::render_view->set_blend(1.5f);
					ap::interfaces::model_render->forced_material_override(crystal_blue);
					original_draw_model_execute(ecx, context, state, render_info, matrix);
					
					ap::interfaces::render_view->set_blend(0.5f);
					ap::interfaces::model_render->forced_material_override(animated_darude);
					original_draw_model_execute(ecx, context, state, render_info, matrix);
					
				}
			}
		}
		
		/* pointer to the entity (clients) */
		ap::sdk::c_base_entity* mango_entity = ap::interfaces::client_entity_list->get_client_entity(render_info.entity_index);
		if (mango_entity != mango_local && render_info.entity_index > 0 && render_info.entity_index < 64 && mango_entity->get_team_num() != mango_local->get_team_num() && mango_local->is_alive() && ap::text_menu::menu::get()._get(L"esp_dont_render_team"))
			return;
	
		original_draw_model_execute(ecx, context, state, render_info, matrix);
		ap::interfaces::model_render->forced_material_override(nullptr);
	}
} // namespace

namespace ap::hooks
{
	void setup()
	{
		client_mode_hook_manager.setup(interfaces::client_mode);
		client_hook_manager.setup(interfaces::client);
		render_view_hook_manager.setup(interfaces::render_view);
		panel_hook_manager.setup(interfaces::panel);
		model_render_hook_manager.setup(interfaces::model_render);
		surface_hook_manager.setup(interfaces::surface);

		original_create_move = client_mode_hook_manager.hook_func<create_move_fn>(24, hooked_create_move);
		original_frame_stage_notify = client_hook_manager.hook_func<frame_stage_notify_fn>(37, hooked_frame_stage_notify);
		original_scene_end = render_view_hook_manager.hook_func<scene_end_fn>(9, hooked_scene_end);
		original_override_view = client_mode_hook_manager.hook_func<override_view_fn>(18, hooked_override_view);
		original_paint_traverse = panel_hook_manager.hook_func<paint_traverse_fn>(41, hooked_paint_traverse);
		original_draw_model_execute = model_render_hook_manager.hook_func<draw_model_execute_fn>(21, hooked_draw_model_execute);
		original_lock_cursor = surface_hook_manager.hook_func<lock_cursor_fn>(67, hooked_lock_cursor);
	}
	void release()
	{
		client_mode_hook_manager.release();
		client_hook_manager.release();
		render_view_hook_manager.release();
		panel_hook_manager.release();
		model_render_hook_manager.release();
		surface_hook_manager.release();
	}
} // namespace ap::hooks