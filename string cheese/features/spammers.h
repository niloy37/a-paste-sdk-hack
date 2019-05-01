#pragma once
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"
#include "../menu.h"
#include "../sdk/c_globals.h"

namespace ap::features::spammers {
	void clan_tag_changer() {
		if (!variables::clan_tag_changer)
			return;
		auto apply = [](const char* name) -> void {
			using Fn = int(__fastcall*)(const char*, const char*);
			static auto fn = reinterpret_cast<Fn>(ap::find_signature("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
			fn(name, name);
		};

		static std::string tag = "    Stick Figure Hake    ";

		static float last_time = 0;

		if (ap::interfaces::globals->curtime > last_time) {
			tag += tag.at(0);
			tag.erase(0, 1);
			std::string dollarsign = u8"//   ";
			dollarsign += tag;
			apply(dollarsign.c_str());

			last_time = ap::interfaces::globals->curtime + 0.9f;
		}

		if (fabs(last_time - ap::interfaces::globals->curtime) > 1.f)
			last_time = ap::interfaces::globals->curtime;
	}
	void on_create_move() {
		clan_tag_changer();
	}
}
