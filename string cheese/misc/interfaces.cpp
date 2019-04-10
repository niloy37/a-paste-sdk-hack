#include "interfaces.h"

#include <Windows.h>
#include <string>

#include "../ap_core.h"
#include "vmt.h"
#include "utils.h"


namespace
{
	void* create_interface(const std::string& module_name, const std::string& interface_name)
	{
		const auto ci_func = reinterpret_cast<void*(*)(const char*, int*)>(GetProcAddress(
			GetModuleHandleA(module_name.c_str()), "CreateInterface"));

		return ci_func(interface_name.c_str(), 0);
	}
	void* bf_create_interface(const std::string& module_name, const std::string& interface_name)
	{
		const auto ci_func = reinterpret_cast<void*(*)(const char*, int*)>(GetProcAddress(
			GetModuleHandleA(module_name.c_str()), "CreateInterface"));

		char buffer[256];
		for (int i = 0; i < 10; i++)
		{
			for (int x = 0; x < 10; x++)
			{
				for (int y = 0; y < 10; y++)
				{
					sprintf_s(buffer, "%s%i%i%i", interface_name.c_str(), i, x, y);

					if (void* interface_addr = ci_func(buffer, 0); interface_addr)
					{
						ap::core::log("found interface: \"%s\" - \"%s\"", module_name.c_str(), buffer);
						return interface_addr;
					}
				}
			}
		}

		ap::core::log("failed to find interface - \"%s\"", interface_name.c_str());
		return nullptr;
	}
} // namespace

namespace ap::interfaces
{
	sdk::c_client* client = nullptr;
	sdk::c_client_entity_list* client_entity_list = nullptr;
	sdk::c_client_mode* client_mode = nullptr;
	sdk::c_client_state* client_state = nullptr;
	sdk::c_render_view* render_view = nullptr;
	sdk::c_engine* engine = nullptr;
	sdk::c_panel* panel = nullptr;
	sdk::c_surface* surface = nullptr;
	sdk::c_input* input = nullptr;
	sdk::c_globals* globals = nullptr;
	sdk::c_model_info* model_info = nullptr;
	sdk::c_trace* trace = nullptr;
	sdk::c_physics_props* physics_props = nullptr;
	sdk::c_cvar* cvar = nullptr;
	sdk::c_debug_overlay* debug_overlay = nullptr;
	sdk::c_model_render* model_render = nullptr;
	sdk::c_material_system* material_system = nullptr;
	sdk::c_prediction* prediction = nullptr;
	sdk::c_game_movement* game_movement = nullptr;
	sdk::c_move_helper* move_helper = nullptr;
	sdk::c_input_system* input_system = nullptr;
	sdk::c_game_types* game_types = nullptr;

	void setup()
	{
		cvar = static_cast<sdk::c_cvar*>(bf_create_interface("vstdlib.dll", "VEngineCvar"));
		client = static_cast<sdk::c_client*>(bf_create_interface("client_panorama.dll", "VClient"));
		client_entity_list = static_cast<sdk::c_client_entity_list*>(bf_create_interface("client_panorama.dll", "VClientEntityList"));
		render_view = static_cast<sdk::c_render_view*>(bf_create_interface("engine.dll", "VEngineRenderView"));
		engine = static_cast<sdk::c_engine*>(bf_create_interface("engine.dll", "VEngineClient"));
		panel = static_cast<sdk::c_panel*>(bf_create_interface("vgui2.dll", "VGUI_Panel"));
		surface = static_cast<sdk::c_surface*>(bf_create_interface("vguimatsurface.dll", "VGUI_Surface"));
		model_info = static_cast<sdk::c_model_info*>(bf_create_interface("engine.dll", "VModelInfoClient"));
		trace = static_cast<sdk::c_trace*>(bf_create_interface("engine.dll", "EngineTraceClient"));
		physics_props = static_cast<sdk::c_physics_props*>(bf_create_interface("vphysics.dll", "VPhysicsSurfaceProps"));
		debug_overlay = static_cast<sdk::c_debug_overlay*>(bf_create_interface("engine.dll", "VDebugOverlay"));
		model_render = static_cast<sdk::c_model_render*>(bf_create_interface("engine.dll", "VEngineModel"));
		material_system = static_cast<sdk::c_material_system*>(bf_create_interface("materialsystem.dll", "VMaterialSystem"));
		prediction = static_cast<sdk::c_prediction*>(bf_create_interface("client_panorama.dll", "VClientPrediction"));
		game_movement = static_cast<sdk::c_game_movement*>(bf_create_interface("client_panorama.dll", "GameMovement"));
		input_system = static_cast<sdk::c_input_system*>(bf_create_interface("inputsystem.dll", "InputSystemVersion"));
		game_types = static_cast<sdk::c_game_types*>(bf_create_interface("matchmaking.dll", "VENGINE_GAMETYPES_VERSION"));

		globals = **reinterpret_cast<sdk::c_globals***>(vmt::get_vfunc<uintptr_t>(client, 0) + 0x1B);
		client_mode = **reinterpret_cast<sdk::c_client_mode***>(vmt::get_vfunc<uintptr_t>(client, 10) + 5);
		client_state = **reinterpret_cast<sdk::c_client_state***>(find_signature("engine.dll", "A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0") + 1);

		input = *reinterpret_cast<sdk::c_input**>(find_signature("client_panorama.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
		move_helper = **reinterpret_cast<sdk::c_move_helper***>(find_signature("client_panorama.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 2);
	}
} // namespace ap::interfaces