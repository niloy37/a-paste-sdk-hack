#include "offsets.h"

#include <unordered_map>
#include <string>

#include "../misc/interfaces.h"
#include "../misc/utils.h"

#include "../ap_core.h"
#include "c_client.h"
#include "c_client_class.h"
#include "recv_data.h"


namespace
{
	std::unordered_map<std::string, std::unordered_map<std::string, uintptr_t>> netvar_map;
	std::unordered_map<std::string, uintptr_t> signature_map;
	std::unordered_map<std::string, uintptr_t> offset_map =
	{
		{ "m_bDormant", 0xED },
		{ "m_MoveType", 0x25C },		 
		{ "m_vecVelocity", 0x114 },		 
		{ "m_vecViewOffset", 0x108 },	 
		{ "m_aimPunchAngle", 0x302C },	 
		{ "m_nTickBase", 0x342C },		 
		{ "m_dwBoneMatrix", 0x26A8 },
		{ "deadflag", 0x31D4 },
		{ "m_ArmorValue", 0xB340 },
		{ "m_bHasHelmet", 0xB334 },
		{ "animstate", 0x3900 }
	};

	void parse_table(ap::sdk::c_recv_table* table)
	{
		for (int i = 0; i < table->m_nProps; i++)
		{
			const auto& prop = table->m_pProps[i];
			netvar_map[table->m_pNetTableName][prop.m_pVarName] = prop.m_Offset;

			if (prop.m_pDataTable)
				parse_table(prop.m_pDataTable);
		}
	}
} // namespace

namespace ap::offsets
{
	void setup()
	{
		auto client_class = interfaces::client->get_all_classes();

		// iterate through each client class
		while (client_class)
		{
			parse_table(client_class->m_pRecvTable);

			client_class = client_class->m_pNext;
		}

		signature_map["setAbsOrigin"] = find_signature("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
		signature_map["setAbsAngle"] = find_signature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");
		signature_map["loadFromBuffer"] = find_signature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");
		signature_map["initKeyValues"] = find_signature("client_panorama.dll", "68 ? ? ? ? 8B C8 E8 ? ? ? ? 89 45 FC EB 07 C7 45 ? ? ? ? ? 8B 03 56");
		signature_map["initKeyValues"] = signature_map["initKeyValues"] + 7 + *reinterpret_cast<uintptr_t*>(signature_map["initKeyValues"] + 8) + 5;
		signature_map["glowObjectManager"] = find_signature("client_panorama.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 0x3;
		signature_map["smokeCount"] = *reinterpret_cast<uintptr_t*>(find_signature("client_panorama.dll", "A3 ? ? ? ? 57 8B CB") + 0x1);
		signature_map["invalidateBoneCache"] = find_signature("client_panorama.dll", "80 3D ? ? ? ? 00 74 16 A1");
		signature_map["utilClipTraceToPlayers"] = find_signature("client_panorama.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC D8 ?? ?? ?? 0F 57 C9");
		signature_map["isBreakableEntity"] = find_signature("client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");
		signature_map["predictionRandomSeed"] = find_signature("client_panorama.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2;
	}

	uintptr_t get_netvar(const std::string& table_name, const std::string& prop_name)
	{
		if (auto table = netvar_map.find(table_name); table != netvar_map.end())
		{
			if (auto prop = table->second.find(prop_name); prop != table->second.end())
			{
				core::log("found netvar: \"%s\" - \"%s\" - 0x%X", table_name.c_str(), prop_name.c_str(), prop->second);
				return prop->second;
			}
		}

		core::log("failed to find netvar: \"%s\" - \"%s\"", table_name.c_str(), prop_name.c_str());
		return 0;
	}

	uintptr_t get_netvar_no_print(const std::string& table_name, const std::string& prop_name)
	{
		if (auto table = netvar_map.find(table_name); table != netvar_map.end())
		{
			if (auto prop = table->second.find(prop_name); prop != table->second.end())
			{
				//core::log("found netvar: \"%s\" - \"%s\" - 0x%X", table_name.c_str(), prop_name.c_str(), prop->second);
				return prop->second;
			}
		}

		//core::log("failed to find netvar: \"%s\" - \"%s\"", table_name.c_str(), prop_name.c_str());
		return 0;
	}

	uintptr_t get_offset(const std::string& offset_name)
	{
		if (auto offset = offset_map.find(offset_name); offset != offset_map.end())
		{
			core::log("found offset: \"%s\" - 0x%X", offset_name.c_str(), offset->second);
			return offset->second;
		}

		core::log("failed to find offset: \"%s\"", offset_name.c_str());
		return 0;
	}

	uintptr_t get_signature(const std::string& signature_name)
	{
		if (auto signature = signature_map.find(signature_name); signature != signature_map.end())
		{
			core::log("found signature: \"%s\" - 0x%X", signature_name.c_str(), signature->second);
			return signature->second;
		}

		core::log("failed to find signature: \"%s\"", signature_name.c_str());
		return 0;
	}

} // namespace ap::netvars