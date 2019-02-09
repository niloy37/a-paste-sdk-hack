#pragma once

#include <memory>
#include <unordered_map>


namespace toenail
{
	struct c_command_info;

	class c_persistant_data
	{
	public:
		~c_persistant_data()
		{
			for (auto& data : m_data)
				free(data.second);

			m_data.clear();
		}

		template <typename T, typename... ARGS>
		T* get_data(const c_command_info& command_info, ARGS... args)
		{
			return get_data<T>(command_info.raw_name + std::to_wstring(command_info.command_type), args...);
		}

		template <typename T, typename... ARGS>
		T* get_data(const std::wstring& unique_name, ARGS... args)
		{
			if (auto it = m_data.find(unique_name); it != m_data.end())
				return reinterpret_cast<T*>(it->second);

			const auto data = m_data[unique_name] = malloc(sizeof(T));
			*reinterpret_cast<T*>(data) = T(args...); // epic
			return reinterpret_cast<T*>(data);
		}

	private:
		std::unordered_map<std::wstring, void*> m_data;
	}; inline c_persistant_data g_persistant_data;
} // namespace toenail