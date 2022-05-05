#pragma once
#include "rage/joaat.hpp"

namespace ext
{
	class ysc_file
	{
		std::ifstream m_file;

		inline void fix_pointer(std::uint64_t& ptr)
		{
			ptr &= 0xFFFFFF;
		}

		inline void rotl(std::uint64_t& hash, int rotate)
		{
			rotate %= 64;
			hash = (hash << rotate | hash >> (64 - rotate));
		}

	public:
		std::vector<uint8_t*> m_code_block_list{};
		std::vector<uint8_t*> m_string_block_list{};
		std::vector<uint64_t> m_natives{};
		rage::joaat_t m_name_hash;
		char m_name[0x40];
		uint64_t* m_statics = nullptr;
		uint64_t m_code_page_list_offset;
		uint64_t m_strings_list_offset;
		uint64_t m_statics_offset;
		uint64_t m_natives_list_offset;
		uint64_t m_script_name_offset;
		uint32_t m_code_length;
		uint32_t m_script_parameter_count;
		uint32_t m_static_count;
		uint32_t m_global_count;
		uint32_t m_natives_count;
		uint32_t m_string_size;

		uint32_t m_code_blocks;
		uint32_t m_string_blocks;

		ysc_file(const std::filesystem::path& filename);
		~ysc_file();
	};
}