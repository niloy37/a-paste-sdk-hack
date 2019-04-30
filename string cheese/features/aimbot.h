#pragma once
#include "../misc/singleton.h"
#include "../sdk/c_base_entity.h"
#include "../sdk/c_base_weapon.h"
#include <deque>

namespace ap::features::aimbot {
	class aimbot : public singleton< aimbot > {
		private:
			struct aimbotconfig_t {
				bool m_autoshoot;
				bool m_autowall;
				bool m_autoscope;
				bool m_hitchance;
		
				int m_minimum_damage;
				int m_hitchance_amount;
		
				//std::vector< MultiDropdownItem_t > m_hitscan;
				//std::vector< MultiDropdownItem_t > m_hitscan_history;
				//std::vector< MultiDropdownItem_t > m_multipoint;
				//std::vector< MultiDropdownItem_t > m_baim;
		
				float m_pointscale;
		
				bool m_quickstop;
			}config;
		
			ap::sdk::c_base_weapon * m_local_weapon;
			ap::vec3f m_engine_angles;
			float m_recoil_scale;
			ap::sdk::c_base_entity * m_target;
		
			//std::deque< tickrecord_t > m_track;
		public:
			void create_move( );
		
			void update_config( );
	};
}
