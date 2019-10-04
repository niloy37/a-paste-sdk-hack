#pragma once
#include <list>
#include "misc/defines.h"
#include "misc/vector.h"
#include "misc/interfaces.h"
#include "sdk/c_client_entity_list.h"
#include "sdk/c_engine.h"
#include "sdk/c_user_cmd.h"
#include "sdk/c_base_entity.h"
#include "misc/utils.h"
#include "sdk/c_client_state.h"
#include "sdk/c_globals.h"
#include "sdk/c_player_info.h"
#include "misc/renderer.h"
#include "sdk/materials.h"
#include "sdk/c_surface.h"
#include "toenail/style.h"
#include "sdk/c_trace.h"
#include "menu.h"
#include "../string cheese/variables.h"
namespace ap::features::misc {
	void shiddy_watermark() {
		if (!variables::misc::get().watermark)
			return;
			static const std::wstring& bruh = L"CHUM";
			static const std::wstring& pasted_watermark = L"THIS WATERMARK POO POO";
			ap::vec2i himmeney;
			ap::interfaces::engine->get_screen_size(himmeney);
			ap::renderer::render_filled_rect(ap::vec2i(himmeney[0] - 162, 4),
				ap::vec2i(182 + himmeney[0] - 187, 20), ap::rgba8(177, 0, 0, 180));
			ap::renderer::render_text(ap::vec2i(himmeney[0] - 167 + 7, 6), ap::rgba8(255, 255, 255, 255),
				toenail::window_title_font, pasted_watermark, false, false);
		
	}
}