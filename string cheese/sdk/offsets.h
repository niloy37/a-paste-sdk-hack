#pragma once

#include <string>


namespace ap::sdk
{
	class c_recv_proxy_data;
} // namespace ap::sdk

namespace ap::offsets
{
	void setup();

	uintptr_t get_netvar(const std::string& table_name, const std::string& prop_name);
	uintptr_t get_netvar_no_print(const std::string & table_name, const std::string & prop_name);
	uintptr_t get_offset(const std::string& offset_name);
	uintptr_t get_signature(const std::string& signature_name);
} // namespace ap::netvars