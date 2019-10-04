#pragma once
#include "misc/singleton.h"
namespace variables {
		struct visuals : public singleton<visuals> {
			bool esp_boxes;
			bool esp_corner_boxes;
			bool name_esp;
			bool health_boxes;
			bool backtrack_stick_figure;
			bool dont_render_team;
			bool armour_flags;
			bool visualize_backtrack_dots;
			bool world_modulation;
			bool visualize_backtrack_octagon;
			bool no_scope_lines;
			bool no_scope;
			bool snap_lines;
			bool enemy_aim_positions;
			bool remove_smoke;
			bool no_flash;
			bool force_crosshair;
			bool custom_crosshair;
			bool no_render_scope;
			bool hand_chams;
			bool enemy_chams;
			bool weapon_chams;
		};
		struct legit : public singleton<legit> {
			bool legit_backtrack;
			int legit_backtrack_ticks = 12;
		};
		struct misc : public singleton<misc> {
			bool auto_jump;
			bool legit_auto_strafer;
			bool no_crouch_cooldown;
			bool fast_crouch_loop;
			bool radar_set_spotted;
			bool clan_tag_changer;
			bool post_processing;
			bool watermark;
		};
		struct rage : public singleton<rage> {
			bool no_recoil;
			float AIMBOT_POINTSCALE = 0.4f;
			float AIMBOT_HITCHANCE = 15.f;
			int AIMBOT_MIN_DAMAGE = 5;
			bool auto_stop;
			bool ragebot_head_only;
			bool silent_aim;
			bool ragebot_test;
		};
		struct antiaim : public singleton<antiaim> {
			bool fakelag_adaptive;
			bool desync_freestanding;
			bool emotion_pitch;
			bool slide_walk;
			bool slow_walk;
		};
}
