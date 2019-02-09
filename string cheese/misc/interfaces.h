#pragma once


namespace ap::sdk
{
	class c_client;
	class c_client_entity_list;
	class c_client_mode;
	class c_client_state;
	class c_render_view;
	class c_engine;
	class c_panel;
	class c_surface;
	class c_input;
	class c_globals;
	class c_model_info;
	class c_trace;
	class c_physics_props;
	class c_cvar;
	class c_debug_overlay;
	class c_model_render;
	class c_material_system;
	class c_prediction;
	class c_game_movement;
	class c_move_helper;
} // namespace ap::sdk

namespace ap::interfaces
{
	void setup();

	extern sdk::c_client* client;
	extern sdk::c_client_entity_list* client_entity_list;
	extern sdk::c_client_mode* client_mode;
	extern sdk::c_client_state* client_state;
	extern sdk::c_render_view* render_view;
	extern sdk::c_engine* engine;
	extern sdk::c_panel* panel;
	extern sdk::c_surface* surface;
	extern sdk::c_input* input;
	extern sdk::c_globals* globals;
	extern sdk::c_model_info* model_info;
	extern sdk::c_trace* trace;
	extern sdk::c_physics_props* physics_props;
	extern sdk::c_cvar* cvar;
	extern sdk::c_debug_overlay* debug_overlay;
	extern sdk::c_model_render* model_render;
	extern sdk::c_material_system* material_system;
	extern sdk::c_prediction* prediction;
	extern sdk::c_game_movement* game_movement;
	extern sdk::c_move_helper* move_helper;
} // namespace ap::interfaces