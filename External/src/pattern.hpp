#pragma once
#include "process.hpp"
#define CHUNK_SIZE 0x1000

namespace ext {

	class pattern {
		std::vector<std::optional<uint8_t>> compiled;
		std::string name;
		uintptr_t   address = 0;

	public:
		inline pattern(std::string name, std::string x) :
			name(name)
		{
			compile(x);
			scan();
		}

		inline pattern(std::string name_, uintptr_t addr) {
			name = name_;
			address = addr;
		}

		void compile(const std::string& pat);

		pattern& scan();

		inline pattern add(size_t n) {
			return pattern(name, address + n);
		}

		inline pattern sub(size_t n) {
			return pattern(name, address - n);
		}

		pattern rip();

		inline operator std::uint64_t() const
		{
			return address;
		}
	};
};