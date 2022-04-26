#pragma once
#include "pointers.hpp"
#include "joaat.hpp"

namespace rage
{
	enum class eThreadState : std::uint32_t
	{
		Running,
		Sleeping,
		Killed,
		Paused,
		Breakpoint
	};

	class scrThread 
	{
		std::uint64_t address;
	public:
		inline scrThread(std::uint64_t addr) :
			address(addr)
		{
		}

		inline bool valid() const
		{
			return address != 0;
		}

		inline joaat_t get_hash() const
		{
			return ext::g_process->read<joaat_t>(address + 0x8 + 0x4);
		}

		inline void set_state(eThreadState state)
		{
			ext::g_process->write<eThreadState>(address + 0x8 + 0x8, state);
		}

		inline void reset()
		{
			ext::g_process->write<uint32_t>(address + 0x8 + 0xC, 0);  // IP
			ext::g_process->write<uint32_t>(address + 0x8 + 0x10, 0); // FP
			ext::g_process->write<uint32_t>(address + 0x8 + 0x54, 0); // CATCH IP
			ext::g_process->write<uint32_t>(address + 0x124, 0); // CLEANUP IP
		}

		template <typename T>
		inline T get_static(uint32_t index)
		{
			return ext::g_process->read<T>(ext::g_process->read<joaat_t>(address + 0xB0) + (index * 8));
		}

		template <typename T>
		inline void set_static(uint32_t index, T value)
		{
			ext::g_process->write<T>(ext::g_process->read<joaat_t>(address + 0xB0) + (index * 8), value);
		}

		inline std::uint64_t get_handler_vft()
		{
			return ext::g_process->read<std::uint64_t>(ext::g_process->read<std::uint64_t>(address + 0x110));
		}

		inline void set_handler_vft(uint64_t vft)
		{
			ext::g_process->write<std::uint64_t>(ext::g_process->read<std::uint64_t>(address + 0x110), vft);
		}

		static inline scrThread get_thread_by_hash(joaat_t hash)
		{
			std::uint16_t num_threads = ext::g_process->read<std::uint16_t>(ext::g_pointers->m_script_threads + 10);
			std::uint64_t threads_base = ext::g_process->read<std::uint64_t>(ext::g_pointers->m_script_threads);

			for (int i = 0; i < num_threads; i++)
			{
				scrThread thread(ext::g_process->read<std::uint64_t>(threads_base + i * 8));
				if (thread.get_hash() == hash) return thread;
			}

			return scrThread(0);
		}
	};
}