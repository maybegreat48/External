#pragma once
#include "features.hpp"

#include "self.hpp"
#include "weapons.hpp"
#include "vehicle.hpp"
#include "mobile.hpp"
#include "spawn.hpp"

namespace ext::program
{
	inline void main(p p)
	{
		p.enter("main", 0, 20);

		p.label("main_loop");

		self::godmode(p);
		self::invisibility(p);
		self::no_cops(p);
		self::fast_run(p);

		weapons::infinite_ammo(p);
		weapons::infinite_clip(p);

		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.push(FALSE);
		p.native("IS_PED_IN_ANY_VEHICLE"_n, 2, 1);
		p.jz("skip_vehicle");

		vehicle::godmode(p);
		vehicle::horn_boost(p);
		vehicle::target(p);
		vehicle::repair(p);

		p.label("skip_vehicle");

		mobile::off_radar(p);

		spawn::loop(p);

		p.push(0);
		p.native("WAIT"_n, 1, 0);
		p.jmp("main_loop");

		p.leave(0, 0);
	}
}