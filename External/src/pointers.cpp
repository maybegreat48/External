#include "pointers.hpp"
#include "pattern.hpp"

namespace ext
{
	pointers::pointers() :
		m_is_session_started(pattern("ISA", "40 38 35 ? ? ? ? 75 0E 4C 8B C3 49 8B D7 49 8B CE").add(3).rip()),
		m_script_threads(pattern("ST", "45 33 F6 8B E9 85 C9 B8").sub(4).rip().sub(8)),
		m_script_program_table(pattern("SP", "44 8B 0D ? ? ? ? 4C 8B 1D ? ? ? ? 48 8B 1D ? ? ? ? 41 83 F8 FF 74 3F 49 63 C0 42 0F B6 0C 18 81 E1").add(17).rip()),
		m_native_registration_table(pattern("NH", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A").add(3).rip())
	{
		g_pointers = this;
	}

	pointers::~pointers()
	{
		g_pointers = nullptr;
	}
}