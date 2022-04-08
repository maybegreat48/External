#pragma once
#include "pointers.hpp"
#include "joaat.hpp"

namespace rage
{
	class scrProgram
	{
		std::uint64_t address;
	public:
		inline scrProgram(std::uint64_t addr) :
			address(addr)
		{
		}

		inline bool valid() const
		{
			return address != 0;
		}

		inline joaat_t get_hash() const
		{
			return ext::g_process->read<joaat_t>(address + 0x18);
		}

		// for the internal debugger
		inline void mark_program_as_ours()
		{
			ext::g_process->write<std::uint32_t>(address + 0x28, 0x1337);
		}

		inline std::uint64_t get_code_page(std::uint64_t index) const
		{
			return ext::g_process->read<std::uint64_t>(ext::g_process->read<std::uint64_t>(address + 0x10) + index * 8);
		}

		inline std::uint64_t get_native_table() const
		{
			return ext::g_process->read<std::uint64_t>(address + 0x40);
		}

		static inline scrProgram get_program_by_hash(joaat_t hash)
		{
			std::uint32_t programs = ext::g_process->read<std::uint32_t>(ext::g_pointers->m_script_program_table + 0x18);
			std::uint64_t program_list = ext::g_process->read<std::uint32_t>(ext::g_pointers->m_script_program_table);

			for (int i = 0; i < programs; i++)
			{
				std::uint64_t program = ext::g_process->read<std::uint64_t>(program_list + (i * 0x10));
				joaat_t phash = ext::g_process->read<joaat_t>(program_list + (i * 0x10) + 0xC);

				if (phash == hash) return scrProgram(program);
			}

			return scrProgram(0);
		}
	};
}