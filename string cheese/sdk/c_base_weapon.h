#pragma once

#include "c_base_entity.h"


namespace ap::sdk
{

	struct c_weapon_info
	{
	public:
		char pad_0x0000[0x4]; //0x0000
		char* m_WeaponName; //0x0004 
		char pad_0x0008[0xC]; //0x0008
		__int32 m_MaxClip; //0x0014 
		char pad_0x0018[0xC]; //0x0018
		__int32 m_MaxReservedAmmo; //0x0024 
		char pad_0x0028[0x4]; //0x0028
		char* m_WeaponMdlPath; //0x002C 
		char pad_0x0030[0x4]; //0x0030
		char* m_DropWeaponMdlPath; //0x0034 
		char pad_0x0038[0x48]; //0x0038
		char* m_BulletType; //0x0080 
		char pad_0x0084[0x4]; //0x0084
		char* m_WeaponHudName; //0x0088 
		char pad_0x008C[0x40]; //0x008C
		__int32 m_WeaponType; //0x00CC 
		__int32 m_WeaponPrice; //0x00D0 
		__int32 m_WeaponReward; //0x00D4 
		char* m_WeaponGroupName; //0x00D8 
		char pad_0x00DC[0x10]; //0x00DC
		unsigned char m_FullAuto; //0x00EC 
		char pad_0x00ED[0x3]; //0x00ED
		__int32 m_WeaponDamage; //0x00F0 
		float m_ArmorRatio; //0x00F4 
		float m_Penetration; //0x00F8
		char pad_0x00F8[0x8]; //0x00FC
		float m_WeaponRange; //0x0108 
		float m_RangeModifier; //0x010C 
		char pad_0x0110[0x10]; //0x0110
		unsigned char m_HasSilencer; //0x0120 
		char pad_0x0121[0xF]; //0x0121
		float m_MaxSpeed; //0x0130 
		float m_MaxSpeed2; //0x0134 
		char pad_0x0138[0x4C]; //0x0138
		__int32 m_RecoilSeed; //0x0184 
		char pad_0x0188[0x68]; //0x0188
		char* m_WeaponTracesType; //0x01F0 
		char pad_0x01F4[0x638]; //0x01F4
	};// Size=0x01A8

	enum ItemDefinitionIndex
	{
		WEAPON_NONE = 0,
		WEAPON_DEAGLE,
		WEAPON_ELITE,
		WEAPON_FIVESEVEN,
		WEAPON_GLOCK,
		WEAPON_AK47 = 7,
		WEAPON_AUG,
		WEAPON_AWP,
		WEAPON_FAMAS,
		WEAPON_G3SG1,
		WEAPON_GALILAR = 13,
		WEAPON_M249,
		WEAPON_M4A1 = 16,
		WEAPON_MAC10,
		WEAPON_P90 = 19,
		WEAPON_MP5SD = 23,
		WEAPON_UMP45,
		WEAPON_XM1014,
		WEAPON_BIZON,
		WEAPON_MAG7,
		WEAPON_NEGEV,
		WEAPON_SAWEDOFF,
		WEAPON_TEC9,
		WEAPON_TASER,
		WEAPON_HKP2000,
		WEAPON_MP7,
		WEAPON_MP9,
		WEAPON_NOVA,
		WEAPON_P250,
		WEAPON_SCAR20 = 38,
		WEAPON_SG553,
		WEAPON_SSG08,
		WEAPON_KNIFEGG,
		WEAPON_KNIFE,
		WEAPON_FLASHBANG,
		WEAPON_HEGRENADE,
		WEAPON_SMOKEGRENADE,
		WEAPON_MOLOTOV,
		WEAPON_DECOY,
		WEAPON_INCGRENADE,
		WEAPON_C4,
		WEAPON_HEALTHSHOT = 57,
		WEAPON_KNIFE_T = 59,
		WEAPON_M4A1_SILENCER,
		WEAPON_USP_SILENCER,
		WEAPON_CZ75A = 63,
		WEAPON_REVOLVER,
		WEAPON_TAGRENADE = 68,
		WEAPON_FISTS,
		WEAPON_BREACHCHARGE,
		WEAPON_TABLET = 72,
		WEAPON_MELEE = 74,
		WEAPON_AXE,
		WEAPON_HAMMER,
		WEAPON_SPANNER = 78,
		WEAPON_KNIFE_GHOST = 80,
		WEAPON_FIREBOMB,
		WEAPON_DIVERSION,
		WEAPON_FRAG_GRENADE,
		WEAPON_KNIFE_BAYONET = 500,
		WEAPON_KNIFE_FLIP = 505,
		WEAPON_KNIFE_GUT,
		WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_M9_BAYONET,
		WEAPON_KNIFE_TACTICAL,
		WEAPON_KNIFE_FALCHION = 512,
		WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
		WEAPON_KNIFE_BUTTERFLY,
		WEAPON_KNIFE_PUSH,
		WEAPON_KNIFE_URSUS = 519,
		WEAPON_KNIFE_GYPSY_JACKKNIFE,
		WEAPON_KNIFE_STILETTO = 522,
		WEAPON_KNIFE_WIDOWMAKER
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
		c_base_weapon* get_weapon()
		{
			static auto offset = offsets::get_netvar("DT_CSPlayer", "m_hActiveWeapon") & 0xFFF;
			return reinterpret_cast<c_base_weapon*>(ap::interfaces::client_entity_list->get_client_entity(offset));
		}
		c_weapon_info* get_weapon_info()
		{
			using fn = c_weapon_info*(__thiscall*)(void*);
			return vmt::get_vfunc<fn>(this, 454)(this);
		}

	private:
	};
} // namespace ap::sdk