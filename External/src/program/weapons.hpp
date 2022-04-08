#pragma once
#include "features.hpp"

namespace ext::program::weapons
{
	inline void infinite_ammo(p p)
	{
		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.local_load((uint16_t)features::weapons_infinite_ammo);
		p.push(NULL);
		p.native("SET_PED_INFINITE_AMMO"_n, 3, 0);
	}

	inline void infinite_clip(p p)
	{
		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.local_load((uint16_t)features::weapons_infinite_clip);
		p.native("SET_PED_INFINITE_AMMO_CLIP"_n, 2, 0);
	}
}