#pragma once
#include "pointers.hpp"
#include "rage/script_thread.hpp"
#include "rage/script_program.hpp"
#include "rage/natives.hpp"
#include "ysc.hpp"

#include "program/features.hpp"

namespace ext
{
	class script_loader
	{
		std::map<uint64_t, uint64_t> m_handler_cache{};
		rage::scrNativeRegistrationTable m_native_registration;
		rage::joaat_t m_required_script;
		rage::scrThread m_thread{0};
		rage::scrProgram m_program{0};
		ysc::program m_bytecode;

	public:
		script_loader();
		~script_loader();

		void find_thread();
		void cache_handlers();
		void initalize_thread();
		ysc::program build_program();

		template <typename T>
		inline T get_feature(program::features feat)
		{
			return m_thread.get_static<T>((std::uint32_t)feat);
		}

		template <typename T>
		inline void set_feature(program::features feat, T value)
		{
			m_thread.set_static<T>((std::uint32_t)feat, value);
		}
	};

	inline script_loader* g_script_loader;
}