#pragma once
#include "features.hpp"

namespace ext::program::self
{
	inline void godmode(p p) 
	{
		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.local_load((uint16_t)features::self_godmode);
		p.native("SET_ENTITY_INVINCIBLE"_n, 2, 0);
	}

	inline void invisibility(p p)
	{
		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.local_load((uint16_t)features::self_invisible);
		p.inot();
		p.push(FALSE);
		p.native("SET_ENTITY_VISIBLE"_n, 3, 0);
	}

	inline void no_cops(p p)
	{
		p.local_load((uint16_t)features::self_no_cops);
		p.jz("skip_no_cops");

		p.native("PLAYER_ID"_n, 0, 1);
		p.push(0);
		p.push(FALSE);
		p.native("SET_PLAYER_WANTED_LEVEL"_n, 3, 0);

		p.label("skip_no_cops");
	}

	inline void fast_run(p p)
	{
		p.local_load((uint16_t)features::self_fast_run);
		p.jz("skip_fast_run");

		p.native("PLAYER_PED_ID"_n, 0, 1);		
		p.native("IS_PED_RUNNING"_n, 1, 1);

		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.native("IS_PED_SPRINTING"_n, 1, 1);

		p.iadd(); // !(TASK::IS_PED_RUNNING(playerPed) || TASK::IS_PED_SPRINTING(playerPed)) -> skip
		p.jz("skip_fast_run");

		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.native("IS_PED_RUNNING_RAGDOLL_TASK"_n, 1, 1);
		p.inot();
		p.jz("skip_fast_run");

		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.push(1);
		p.pushf(0.0f);
		p.pushf(1.5f); // force
		p.pushf(0.0f);

		p.pushf(0.0f);
		p.pushf(0.0f);
		p.pushf(0.0f);

		p.push(0);
		p.push(TRUE);
		p.push(TRUE);
		p.push(TRUE);
		p.push(FALSE);
		p.push(TRUE);

		p.native("APPLY_FORCE_TO_ENTITY"_n, 14, 0);

		p.label("skip_fast_run");
	}
}