#pragma once
#include "pointer.hpp"

namespace ext
{
	class pointers
	{
	public:
		explicit pointers();
		~pointers();
	public:
		pointer<bool> m_is_session_started;
		pointer<uint64_t> m_script_threads;
		pointer<uint64_t> m_script_program_table;
		pointer<uint64_t> m_native_registration_table;
	};

	inline pointers* g_pointers;
}