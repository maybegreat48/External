#pragma once
#include "process.hpp"

namespace rage
{
	// https://www.unknowncheats.me/forum/grand-theft-auto-v/473487-entry-dumper-gta5.html
	// https://www.unknowncheats.me/forum/3036413-post862.html

	class scrNativeRegistrationTable
	{
		template<typename T>
		struct Enc {
		private:
			uint32_t a{};
			uint32_t b{};
			uint32_t c{};
			uint8_t _pad[4]{};

		public:
			[[nodiscard]] inline T decrypt(uint64_t _this) const 
			{
				auto const nonce = _this ^ c;
				auto const value = static_cast<uint32_t>(nonce ^ a) | (nonce ^ b) << 32u;
				if constexpr (std::is_integral_v<T>) {
					return value;
				}
				else {
					return reinterpret_cast<T>(value);
				}
			}
		};

		struct NativeRegistration
		{
			Enc<uint64_t> next;
			uint64_t handlers[7]{};
			uint32_t numEntries1 = 0;
			uint32_t numEntries2 = 0;
			uint32_t pad = 0;
			std::array<Enc<uint64_t>, 7> hashes;
		};

		std::uint64_t address;
	public:
		scrNativeRegistrationTable(std::uint64_t addr) :
			address(addr)
		{

		}

		inline std::uint64_t get_entrypoint_from_hash(std::uint64_t translated_hash)
		{
			for (size_t j = 0; j < 256; ++j) {
				size_t tableptr = ext::g_process->read<size_t>(address + 8 * j);

				while (tableptr) {
					struct NativeRegistration entry{};
					ext::g_process->read<NativeRegistration>(tableptr, entry);

					const uint32_t numEntries = entry.numEntries1 ^ entry.numEntries2 ^ uint32_t(tableptr + offsetof(NativeRegistration, numEntries1));

					for (size_t i = 0; i < numEntries; i++) {
						const uint64_t entryHash = entry.hashes[i].decrypt(tableptr + 0x54 + (i * 0x10));

						if (entryHash == translated_hash)
						{
							return entry.handlers[i];
						}
					}
					tableptr = entry.next.decrypt(tableptr);
				}
			}

			LOG(WARNING) << "Cannot find handler for translated hash " << HEX_TO_UPPER(translated_hash);
		}
	};
}