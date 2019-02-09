#pragma once

#include "c_base_entity.h"


namespace ap::sdk
{
	struct c_weapon_info
	{
	public:
		char _0x0000[20];
		__int32 max_clip;			//0x0014 
		char _0x0018[12];
		__int32 max_reserved_ammo;	//0x0024 
		char _0x0028[96];
		char* hud_name;				//0x0088 
		char* weapon_name;			//0x008C 
		char _0x0090[60];
		__int32 m_WeaponType;				//0x00CC 
		__int32 price;				//0x00D0 
		__int32 reward;				//0x00D4 
		char _0x00D8[20];
		BYTE full_auto;				//0x00EC 
		char _0x00ED[3];
		__int32 m_Damage;				//0x00F0 
		float m_ArmorRatio;			//0x00F4 
		__int32 bullets;			//0x00F8 
		float m_Penetration;			//0x00FC 
		char _0x0100[8];
		float m_Range;				//0x0108 
		float m_RangeModifier;		//0x010C 
		char _0x0110[16];
		bool m_HasSilencer;				//0x0120 
		char _0x0121[15];
		float max_speed;			//0x0130 
		float max_speed_alt;		//0x0134 
		char _0x0138[76];
		__int32 recoil_seed;		//0x0184 
		char _0x0188[32];
	};// Size=0x01A8

	enum ItemDefinitionIndex
	{
		weapon_none,
		weapon_deagle,
		weapon_elite,
		weapon_fiveseven,
		weapon_glock,
		weapon_ak47 = 7,
		weapon_aug,
		weapon_awp,
		weapon_famas,
		weapon_g3sg1,
		weapon_galilar = 13,
		weapon_m249,
		weapon_m4a4 = 16,
		weapon_mac10,
		weapon_p90 = 19,
		weapon_ump45 = 24,
		weapon_xm1014,
		weapon_bizon,
		weapon_mag7,
		weapon_negev,
		weapon_sawedoff,
		weapon_tec9,
		weapon_taser,
		weapon_p2000,
		weapon_mp7,
		weapon_mp9,
		weapon_nova,
		weapon_p250,
		weapon_scar20 = 38,
		weapon_sg556,
		weapon_ssg08,
		weapon_knife_ct = 42,
		weapon_flashbang,
		weapon_hegrenade,
		weapon_smokegrenade,
		weapon_molotov,
		weapon_decoy,
		weapon_incgrenade,
		weapon_c4,
		weapon_knife_t = 59,
		weapon_m4a1s = 60,
		weapon_usp = 61,
		weapon_cz75 = 63,
		weapon_revolver,
		weapon_tagrenade = 68,
		weapon_bayonet = 500,
		weapon_flip = 505,
		weapon_gut,
		weapon_karambit,
		weapon_m9bayonet,
		weapon_huntsman,
		weapon_falchion = 512,
		weapon_bowie = 514,
		weapon_butterfly = 515,
		weapon_pushdagger,
		studded_bloodhound_gloves = 5027,
		sporty_gloves = 5030,
		slick_gloves = 5031,
		leather_handwraps = 5032,
		motorcycle_gloves = 5033,
		specialist_gloves = 5034
	};

	class c_base_weapon : public c_base_entity
	{
	public:
		void update_accuracy_penalty()
		{
			using fn = void(__thiscall*)(void*);
			vmt::get_vfunc<fn>(this, 472)(this);
		}
		short get_item_definition_index() const
		{
			static const auto offset = offsets::get_netvar("DT_EconEntity", "m_AttributeManager") + 
				offsets::get_netvar("DT_AttributeContainer", "m_Item") + 
				offsets::get_netvar("DT_ScriptCreatedItem", "m_iItemDefinitionIndex");
			return *reinterpret_cast<short*>(uintptr_t(this) + offset);
		}
		int get_ammo() const
		{
			static const auto offset = offsets::get_netvar("DT_BaseCombatWeapon", "m_iClip1");
			return *reinterpret_cast<int*>(uintptr_t(this) + offset);
		}
		float get_next_primary_attack() const
		{
			static auto offset = offsets::get_netvar("DT_LocalActiveWeaponData", "m_flNextPrimaryAttack");
			return *reinterpret_cast<float*>(uintptr_t(this) + offset);
		}
		float get_next_secondary_attack() const
		{
			static auto offset = offsets::get_netvar("DT_LocalActiveWeaponData", "m_flNextSecondaryAttack");
			return *reinterpret_cast<float*>(uintptr_t(this) + offset);
		}
		float get_inaccuracy()
		{
			using fn = float(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 471)(this);
		}
		float get_spread_cone()
		{
			using fn = float(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 440)(this);
		}
		c_weapon_info* get_weapon_info()
		{
			using fn = c_weapon_info*(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 448)(this);
		}

	private:
	};
} // namespace ap::sdk