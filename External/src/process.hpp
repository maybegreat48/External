#pragma once

namespace ext 
{
	class process;
	inline process* g_process;

	class process 
	{
		HANDLE m_handle;
		uint32_t m_pid;

	public:
		uintptr_t m_base_address;
		size_t m_size;

		process(uint32_t pid) :
			m_pid(pid)
		{
			m_handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			if (!m_handle) {
				throw std::exception("Failed to open handle");
			}

			HMODULE modules[0xFF];
			MODULEINFO module_info;
			DWORD _;

			if (!::EnumProcessModulesEx(m_handle, modules, sizeof(modules), &_, LIST_MODULES_64BIT))
				throw std::exception("Failed to enumerate modules");

			m_base_address = (uintptr_t)modules[0];

			if (!::GetModuleInformation(m_handle, modules[0], &module_info, sizeof(module_info)))
				throw std::exception("Failed to get module info");

			m_size = module_info.SizeOfImage;

			g_process = this;
		}

		~process() {
			::CloseHandle(m_handle);
			g_process = nullptr;
		}

		inline bool is_running() 
		{
			uint32_t exit_code;
			if (::GetExitCodeProcess(m_handle, (LPDWORD)&exit_code)) {
				return exit_code == STATUS_PENDING;
			}
			else {
				return false;
			}
		}

		inline void set_paused(bool paused)
		{
			if (paused)
			{
				::SuspendThread(m_handle);
			}
			else
			{
				::ResumeThread(m_handle);
			}
		}

		inline bool read_raw_ex(uintptr_t base, size_t size, void* out) 
		{
			return ::ReadProcessMemory(m_handle, (LPCVOID)base, out, size, nullptr);
		}

		inline void read_raw(uintptr_t base, size_t size, void* out) 
		{
			if (!::ReadProcessMemory(m_handle, (LPCVOID)base, out, size, nullptr)) 
			{
				throw std::exception("RPM failed");
			}
		}

		inline void write_raw(uintptr_t base, size_t size, void* buf) 
		{
			if (!::WriteProcessMemory(m_handle, (LPVOID)base, buf, size, nullptr)) 
			{
				throw std::exception("RPM failed");
			}
		}

		template <typename T>
		inline void read(uintptr_t base, T& ref) 
		{
			read_raw(base, sizeof(T), &ref);
		}

		template <typename T>
		inline T read(uintptr_t base) 
			requires(std::is_scalar_v<T>)
		{
			T out{};
			read_raw(base, sizeof(T), &out);
			return out;
		}

		template <typename T>
		inline void write(uintptr_t base, const T& ref) {
			write_raw(base, sizeof(T), (void*)&ref);
		}
	};
}