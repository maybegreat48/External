#include "pattern.hpp"

namespace ext
{
	void pattern::compile(const std::string& pat) {
		int hexchar = 0;
		bool first = true;
		bool lastwaswc = false;

		auto to_hex = [&](char c) -> std::uint8_t
		{
			switch (std::toupper(c))
			{
			case '0':
				return static_cast<std::uint8_t>(0);
			case '1':
				return static_cast<std::uint8_t>(1);
			case '2':
				return static_cast<std::uint8_t>(2);
			case '3':
				return static_cast<std::uint8_t>(3);
			case '4':
				return static_cast<std::uint8_t>(4);
			case '5':
				return static_cast<std::uint8_t>(5);
			case '6':
				return static_cast<std::uint8_t>(6);
			case '7':
				return static_cast<std::uint8_t>(7);
			case '8':
				return static_cast<std::uint8_t>(8);
			case '9':
				return static_cast<std::uint8_t>(9);
			case 'A':
				return static_cast<std::uint8_t>(10);
			case 'B':
				return static_cast<std::uint8_t>(11);
			case 'C':
				return static_cast<std::uint8_t>(12);
			case 'D':
				return static_cast<std::uint8_t>(13);
			case 'E':
				return static_cast<std::uint8_t>(14);
			case 'F':
				return static_cast<std::uint8_t>(15);
			default:
				return 0;
			}
		};

		for (char c : pat) {
			if (c == '?' && !lastwaswc) {
				lastwaswc = true;
				compiled.push_back(std::nullopt);
			}
			else {
				lastwaswc = false;
				if (c == ' ') continue;
				hexchar += first ? to_hex(c) * 0x10 : to_hex(c);

				if (!first) {
					compiled.push_back(hexchar);
					hexchar = 0;
				}
				first = !first;
			}
		}
	}

	pattern& pattern::scan() {
		size_t i = g_process->m_base_address;
		std::uint8_t* membuf = new std::uint8_t[CHUNK_SIZE];

		address = 0;

		while (address == 0) {
			if (i >= (g_process->m_base_address + g_process->m_size))
				break;

			if (!g_process->read_raw_ex(i, CHUNK_SIZE, membuf))
				goto next;

			for (int j = 0; j < CHUNK_SIZE; j++) {
				for (int k = 0; k < compiled.size(); k++) {
					if (!compiled[k].has_value()) continue;
					if (compiled[k].value() != membuf[j + k]) goto incorrect;
				}

				address = i + j;
				break;

			incorrect: continue;
			}

		next:
			i += CHUNK_SIZE;
		}

		delete[] membuf;

		if (address == NULL)
			throw std::exception(("cannot find pattern " + name).data());

		return *this;
	}

	pattern pattern::rip() {
		return add(g_process->read<int>(address)).add(4);
	}
}