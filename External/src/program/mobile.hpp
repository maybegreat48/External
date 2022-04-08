#pragma once
#include "features.hpp"

namespace ext::program::mobile
{
	inline void off_radar(p p)
	{
		p.local_load((uint16_t)features::mobile_off_radar);
		p.native("PLAYER_ID"_n, 0, 1);
		p.global(2689156);
		p.array(453);
		p.offset_store(209);

		p.native("GET_NETWORK_TIME"_n, 0, 1);
		p.push(1);
		p.iadd();
		p.global(2703656);
		p.offset_store(70);
	}
}