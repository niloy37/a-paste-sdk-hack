#pragma once

#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_model_info.h"
#include "../sdk/offsets.h"
#include "../sdk/c_client_networkable.h"
#include "../sdk/c_client_renderable.h"
#include "../sdk/c_client_thinkable.h"
#include "../sdk/c_client_unknown.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_animstate.h"
#include "../misc/math.h"
namespace ap::sdk
{
	class c_client_class;
	struct c_animstate;

	class c_base_entity : public c_client_unknown, public c_client_renderable, public c_client_networkable {
	public:

		bool is_enemy(c_base_entity* entity) const
		{
			if (get_team_num() != entity->get_team_num())
				return true;

			return false;
		}

		void update_client_side_animations()
		{
			using fn = void(__thiscall*)(void*);
			vmt::get_vfunc<fn>(this, 219)(this);
		}
		void set_client_side_animation(bool value)
		{
			static const auto offset = offsets::get_netvar("DT_BaseAnimating", "m_bClientSideAnimation");
			*reinterpret_cast<bool*>(uintptr_t(this) + offset) = value;
		}
		void set_observer_mode(int mode)
		{
			static const auto offset = offsets::get_netvar("DT_BasePlayer", "m_iObserverMode");
			*reinterpret_cast<int*>(uintptr_t(this) + offset) = mode;
		}
		void set_is_spotted(bool value)
		{
			static const auto offset = offsets::get_netvar("DT_BaseEntity", "m_bSpotted");
			*reinterpret_cast<bool*>(uintptr_t(this) + offset) = value;
		}
		void set_flash_duration(float value)
		{
			static const auto offset = offsets::get_netvar("DT_CSPlayer", "m_flFlashDuration");
			*reinterpret_cast<float*>(uintptr_t(this) + offset) = value;			
		}
		void set_abs_origin(const vec3f& origin)
		{
			using fn = void(__thiscall*)(void*, const vec3f& origin);
			if (static fn func = fn(offsets::get_signature("setAbsOrigin")); func)
				func(this, origin);
		}
		void set_abs_angle(const vec3f& angle)
		{
			using fn = void(__thiscall*)(void*, const vec3f& angle);
			if (static fn func = fn(offsets::get_signature("setAbsAngle")); func)
				func(this, angle);
		}
		void set_eye_angle(const vec3f& angle)
		{
			static const auto offset = offsets::get_netvar("DT_CSPlayer", "m_angEyeAngles");
			*reinterpret_cast<vec3f*>(uintptr_t(this) + offset) = angle;
		}
		bool is_immune() const
		{
			static const auto offset = offsets::get_netvar("DT_CSPlayer", "m_bGunGameImmunity");
			return *reinterpret_cast<bool*>(uintptr_t(this) + offset);
		}
		bool is_dormant() const
		{
			static const auto offset = offsets::get_offset("m_bDormant");
			return *reinterpret_cast<bool*>(uintptr_t(this) + offset);
		}
		bool is_scoped() 
		{
			static const auto offset = offsets::get_netvar("DT_CSPlayer", "m_bIsScoped");
			return *reinterpret_cast<bool*>(uintptr_t(this) + offset);
		}
		void is_spotted(bool state) 
		{
			static const auto offset = offsets::get_netvar("DT_BaseEntity", "m_bSpotted");
			*reinterpret_cast<bool*>(uintptr_t(this) + offset) = state;
		}
		bool is_weapon()
		{
			using fn = bool(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 161)(this);
		}
		bool is_bomb_ticking() const
		{
			static const auto offset = offsets::get_netvar("DT_PlantedC4", "m_bBombTicking");
			return *reinterpret_cast<bool*>(uintptr_t(this) + offset);
		}
		bool is_bomb_defused() const
		{
			static const auto offset = offsets::get_netvar("DT_PlantedC4", "m_bBombDefused");
			return *reinterpret_cast<bool*>(uintptr_t(this) + offset);
		}
		bool has_helmet()
		{
			static const auto offset = offsets::get_netvar("DT_CSPlayer", "m_bHasHelmet");
			return *reinterpret_cast<bool*>(uintptr_t(this) + offset);
		}
		bool is_player() 
		{
			using fn = bool(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 153)(this);
		}
		bool is_alive()
		{
			return this->get_health() > 0;
		}
		bool is_valid(c_base_entity* local_player = nullptr, bool is_player = true, bool dormant = true, bool team_check = true) {
			if (!this)
				return false;

			if (dormant)
				if (this->IsDormant())
					return false;

			if (team_check && local_player)
				if (this->get_team_num() == local_player->get_team_num())
					return false;

			if (is_player)
				if (!this->is_player())
					return false;

			if (this->get_health() <= 0)
				return false;

			return true;
		}
		int get_health() const
		{
			static const auto offset = offsets::get_netvar("DT_BasePlayer", "m_iHealth");
			return *reinterpret_cast<int*>(uintptr_t(this) + offset);
		}
		int get_flags() const
		{
			static const auto offset = offsets::get_netvar("DT_BasePlayer", "m_fFlags");
			return *reinterpret_cast<int*>(uintptr_t(this) + offset);
		}
		int get_team_num() const
		{
			static const auto offset = offsets::get_netvar("DT_BaseEntity", "m_iTeamNum");
			return *reinterpret_cast<int*>(uintptr_t(this) + offset);
		}
		int get_armor() const
		{
			static const auto offset = offsets::get_netvar("DT_CSPlayer", "m_ArmorValue");
			return *reinterpret_cast<int*>(uintptr_t(this) + offset);
		}
		int get_move_type() const
		{
			static const auto offset = offsets::get_offset("m_MoveType");
			return *reinterpret_cast<int*>(uintptr_t(this) + offset);
		}
		int get_active_weapon() const
		{
			static const auto offset = offsets::get_netvar("DT_BaseCombatCharacter", "m_hActiveWeapon");
			return *reinterpret_cast<int*>(uintptr_t(this) + offset) & 0xFFF;
		}
		int get_tick_base() const
		{
			static const auto offset = offsets::get_offset("m_nTickBase");
			return *reinterpret_cast<int*>(uintptr_t(this) + offset);
		}
		int get_index() const
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + 0x64);
		}
		int get_num_anim_overlays() const
		{
			return *(int*)((DWORD)this + 0x298C);
		}
		animation_layer* get_anim_overlays()
		{
			return *(animation_layer * *)((DWORD)this + 0x2980);
		}
		animation_layer get_anim_overlay(int i) {
			if (i < 15)
				return get_anim_overlays()[i];
		}
		int draw_model(int flags, uint8_t alpha)
		{
			void* renderable = static_cast<void*>(this + 0x4);
			using fn = int(__thiscall*)(void*, int, uint8_t);
			return vmt::get_vfunc<fn>(renderable, 9)(renderable, flags, alpha);
		}
		float get_next_attack() const
		{
			static const auto offset = offsets::get_netvar("DT_BCCLocalPlayerExclusive", "m_flNextAttack");
			return *reinterpret_cast<float*>(uintptr_t(this) + offset);
		}
		float get_crouch_amount() const
		{
			static const auto offset = offsets::get_netvar("DT_BasePlayer", "m_flDuckAmount");
			return *reinterpret_cast<float*>(uintptr_t(this) + offset);
		}
		float get_lower_body_yaw() const
		{
			static const auto offset = offsets::get_netvar("DT_CSPlayer", "m_flLowerBodyYawTarget");
			return *reinterpret_cast<float*>(uintptr_t(this) + offset);
		}
		float get_simulation_time() const
		{
			static const auto offset = offsets::get_netvar("DT_BaseEntity", "m_flSimulationTime");
			return *reinterpret_cast<float*>(uintptr_t(this) + offset);
		}
		float get_bomb_blow_time() const
		{
			static const auto offset = offsets::get_netvar("DT_PlantedC4", "m_flC4Blow");
			return *reinterpret_cast<float*>(uintptr_t(this) + offset);
		}
		float flash_duration(float value) const
		{
			static const auto offset = offsets::get_netvar("DT_CSPlayer", "m_flFlashDuration");
			return *reinterpret_cast<float*>(uintptr_t(this) + offset) = value;
		}
		float crouch_amount() const
		{
			static const auto offset = offsets::get_netvar("DT_BasePlayer", "m_flDuckAmount");
			return *reinterpret_cast<float*>(uintptr_t(this) + offset);
		}
		float* get_pose_params() const
		{
			static const auto offset = offsets::get_netvar("DT_BaseAnimating", "m_flPoseParameter");
			return reinterpret_cast<float*>(uintptr_t(this) + offset);
		}
		vec3f get_abs_origin()
		{
			using fn = vec3f&(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 10)(this);
		}
		vec3f get_vec_origin() const
		{
			static const auto offset = offsets::get_netvar("DT_BaseEntity", "m_vecOrigin");
			return *reinterpret_cast<vec3f*>(uintptr_t(this) + offset);
		}
		vec3f get_obb_min_s() const
		{
			static const auto offset = offsets::get_netvar("DT_BaseEntity", "m_vecMins");
			return *reinterpret_cast<vec3f*>(uintptr_t(this) + offset);
		}
		vec3f get_obb_max_s() const
		{
			static const auto offset = offsets::get_netvar("DT_BaseEntity", "m_vecMaxs");
			return *reinterpret_cast<vec3f*>(uintptr_t(this) + offset);
		}
		vec3f get_eye_angle() const
		{
			static const auto offset = offsets::get_netvar("DT_CSPlayer", "m_angEyeAngles");
			return *reinterpret_cast<vec3f*>(uintptr_t(this) + offset);
		}
		vec3f get_velocity() const
		{
			static const auto offset = offsets::get_offset("m_vecVelocity");
			return *reinterpret_cast<vec3f*>(uintptr_t(this) + offset);
		}
		vec3f get_view_offset() const
		{
			static const auto offset = offsets::get_offset("m_vecViewOffset");
			return *reinterpret_cast<vec3f*>(uintptr_t(this) + offset);
		}
		vec3f get_aim_punch() const
		{
			static const auto offset = offsets::get_offset("m_aimPunchAngle");
			return *reinterpret_cast<vec3f*>(uintptr_t(this) + offset);
		}
		vec3f get_eye_position()
		{
			vec3f tmp;
			using fn = void(__thiscall*)(void*, vec3f&);
			vmt::get_vfunc<fn>(this, 279)(this, tmp);
			return tmp;
		}
		model_t* get_model() const
		{
			void* renderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
			using fn = model_t* (__thiscall*)(void*);
			return vmt::get_vfunc<fn>(renderable, 8)(renderable);
		}
		matrix3x4_t get_bone_matrix(int index) const
		{
			static const auto offset = offsets::get_offset("m_dwBoneMatrix");
			const auto base = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + offset);
			return *reinterpret_cast<matrix3x4_t*>(base + (sizeof(matrix3x4_t) * index));
		}
		matrix3x4_t& get_rgflcoordinateframe() const
		{
			static const auto offset = offsets::get_netvar("DT_CSPlayer", "m_CollisionGroup") - 0x30;
			return *reinterpret_cast<matrix3x4_t*>(uintptr_t(this) + offset);
		}
		c_client_class* get_client_class()
		{
			void* networkable = static_cast<void*>(this + 0x8);
			using fn = c_client_class*(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(networkable, 2)(networkable);
		}
		mstudiobbox_t* get_hitbox(int index) const
		{
			if (is_dormant() || get_health() <= 0)
				return nullptr;

			const auto model = get_model();
			if (!model)
				return nullptr;

			const auto studio_hdr = interfaces::model_info->get_studio_hdr(model);
			if (!studio_hdr)
				return nullptr;

			const auto set = studio_hdr->GetHitboxSet(0);
			if (!set)
				return nullptr;

			if (index >= set->numhitboxes || index < 0)
				return nullptr;

			return set->GetHitbox(index);
		}
		c_animstate* get_animstate()
		{
			static const auto offset = offsets::get_offset("animstate");
			return *reinterpret_cast<c_animstate**>(uintptr_t(this) + offset);
		}
		float get_max_desync_delta() {
			auto anim_state = this->get_animstate();
			if (!anim_state)
				return 0.f;

			float duck_amount = anim_state->m_fDuckAmount;
			float speed_fraction = ap::max< float >(0, ap::min< float >(anim_state->m_flFeetSpeedForwardsOrSideWays, 1));
			float speed_factor = ap::max< float >(0, ap::min< float >(1, anim_state->m_flFeetSpeedUnknownForwardOrSideways));

			float yaw_modifier = (((anim_state->m_flStopToFullRunningFraction * -0.3f) - 0.2f) * speed_fraction) + 1.0f;

			if (duck_amount > 0.f) {
				yaw_modifier += ((duck_amount * speed_factor) * (0.5f - yaw_modifier));
			}

			return anim_state->m_vVelocityY * yaw_modifier;
		}
	private:
		// To get value from the pointer itself
		template<class T>
		T get_value(const int offset)
		{
			return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
		}
	};

} // namespace ap::sdk