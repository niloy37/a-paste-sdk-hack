#pragma once

#include <unordered_map>

#include <Windows.h>


namespace ap::vmt
{
	template <class T>
	static T get_vfunc(void* instance, int index)
	{
		const auto vtable = *static_cast<void***>(instance);
		return T(vtable[index]);
	}

	class c_vmt_hook_manager
	{
	public:
		c_vmt_hook_manager() = default;
		c_vmt_hook_manager(void* instance) { setup(instance); }
		~c_vmt_hook_manager() { release(); }

		void setup(void* instance)
		{
			m_instance = instance;
			m_vtable = *static_cast<void***>(instance);

			m_size = 0;
			for (void* curr_func = m_vtable[m_size]; curr_func != nullptr;)
			{
				MEMORY_BASIC_INFORMATION mbi;
				if (!VirtualQuery(curr_func, &mbi, sizeof(mbi)))
					break;

				if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
					break;

				if (!(mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_READONLY | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_WRITECOPY)))
					break;

				m_size++;
				curr_func = m_vtable[m_size];
			}
		}
		void release()
		{
			if (!m_instance || !m_vtable)
				return;

			// restore every func
			for (const auto &func : m_original_funcs)
			{
				unsigned long old_protection, tmp;
				VirtualProtect(&m_vtable[func.first], sizeof(void*), PAGE_EXECUTE_READWRITE, &old_protection);
				m_vtable[func.first] = func.second;
				VirtualProtect(&m_vtable[func.first], sizeof(void*), old_protection, &tmp);
			}

			m_instance = nullptr;
			m_vtable = nullptr;
			m_size = 0;
			m_original_funcs.clear();
		}

		template <class T>
		T hook_func(std::size_t index, void* func)
		{
			// index is out of range or null function pointer
			if (index < 0 || index >= m_size || !func)
				return T(nullptr);

			// get the original (this assumes that you dont try to hook the function twice like a retard)
			const auto original = T(m_vtable[index]);
			m_original_funcs[index] = original;

			unsigned long old_protection, tmp;
			VirtualProtect(&m_vtable[index], sizeof(void*), PAGE_EXECUTE_READWRITE, &old_protection);
			m_vtable[index] = func;
			VirtualProtect(&m_vtable[index], sizeof(void*), old_protection, &tmp);

			return original;
		}

		template <class T>
		T get_original_func(std::size_t index)
		{
			// index is out of range
			if (index < 0 || index >= m_size)
				return T(nullptr);

			// function isn't even hooked - just return the one in the vtable
			if (m_original_funcs.find(index) == m_original_funcs.end())
				return T(m_vtable[index]);

			return T(m_original_funcs[index]);
		}

	private:
		void* m_instance = nullptr;
		void** m_vtable = nullptr;
		std::size_t m_size = 0;
		std::unordered_map<int, void*> m_original_funcs;
	};
} // namespace ap::vmt