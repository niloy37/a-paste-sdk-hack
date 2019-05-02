#pragma once
#include "../misc/singleton.h"
#include "../sdk/c_base_entity.h"
#include "../sdk/c_base_weapon.h"
#include <deque>
#include "../sdk/c_user_cmd.h"
#include "../sdk/c_globals.h"
namespace ap::features::aimbot {
	void run(sdk::c_user_cmd* cmd);
	class ragebot : public singleton <ragebot>{
	public:
		void run(ap::sdk::c_user_cmd* cmd);
		void no_recoil(ap::sdk::c_user_cmd* cmd);
		void begin(ap::sdk::c_user_cmd* cmd);
		vec3f GetHitboxPosition(ap::sdk::c_base_entity* pEntity, int Hitbox);
		vec3f head_hitbox(ap::sdk::c_base_entity* e);
	};
	struct lag_record_t
	{
		lag_record_t(ap::sdk::c_base_entity* player)
			: m_bones{ } {

			// save all data here.
			m_simtime = player->get_simulation_time();
			m_origin = player->get_abs_origin();
			m_lby = player->get_lower_body_yaw();
			m_flags = player->get_flags();
			m_vel = player->get_velocity();
			player->SetupBones(m_bones.data(), 128, 0x100, ap::interfaces::globals->curtime);
		}

		bool is_valid() const;

		int						m_flags;
		vec3f					m_origin;
		float					m_lby;
		float					m_simtime;
		vec3f					m_vel;

	//	c_utl_vector< float >	m_poses;

		std::array< matrix3x4_t, 128 >	m_bones;
	};
	class c_ragebot : public singleton <c_ragebot>{
		struct rage_t {
			rage_t(ap::sdk::c_base_entity* player, int i, int damage, vec3f point, float dist, matrix3x4_t* matrix) {
				m_player = player;
				index = i;
				m_damage = damage;
				m_bestpoint = point;
				distance = dist;
				mat = matrix;
			}

			vec3f m_bestpoint;
			ap::sdk::c_base_entity* m_player;
			int index;
			int m_damage;
			float distance;
			matrix3x4_t* mat;
		};

		std::vector< rage_t > m_players;
		ap::sdk::c_user_cmd* m_cmd;

		void choose_angles();
		void select_target();
		bool get_points_from_hitbox(ap::sdk::c_base_entity* e, std::vector< int > hitboxes, matrix3x4_t* matrix, std::vector<vec3f>& points, float scale);
		//void quickstop(c_base_combat_weapon* local_weapon);
		bool is_valid(ap::sdk::c_base_entity* player);
		bool hitchance(vec3f& angle, ap::sdk::c_base_entity* ent);
		static std::vector< lag_record_t > get_best_records(std::deque< lag_record_t > records);

	public:
		int shots_fired;
		ap::sdk::c_base_entity* m_last_target = nullptr;

		void work(ap::sdk::c_user_cmd* cmd);
		//void auto_revolver(c_base_combat_weapon* local_weapon, c_user_cmd* cmd);
	};
}
