#pragma once
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <span>

namespace rage
{
	using joaat_t = std::uint32_t;

	inline constexpr char joaat_to_lower(char c)
	{
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}

	inline constexpr joaat_t constexpr_joaat(const std::span<const char>& data)
	{
		joaat_t hash = 0;

		for (std::size_t i = 0; i < data.size() - 1; ++i)
		{
			hash += joaat_to_lower(data[i]);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}
	static_assert(constexpr_joaat("test") == 0x3f75ccc1);

	inline joaat_t joaat(std::string_view str)
	{
		joaat_t hash = 0;

		for (char c : str)
		{
			hash += joaat_to_lower(c);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}

	inline joaat_t joaat(const char* str)
	{
		joaat_t hash = 0;

		while (*str)
		{
			hash += joaat_to_lower(*(str++));
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}
}

#define RAGE_JOAAT(str) std::integral_constant<std::uint32_t, rage::constexpr_joaat(str)>::value
#define $(x) RAGE_JOAAT(#x)