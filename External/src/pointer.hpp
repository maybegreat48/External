#pragma once
#include "process.hpp"

namespace ext 
{
	template <typename T>
	class pointer
	{
		std::uint64_t address;
	public:
		inline pointer(std::uint64_t address) :
			address(address)
		{
		}

		inline T operator*() const
		{
			return g_process->read<T>(address);
		}

		inline operator uint64_t() const
		{
			return address;
		}
	};
}