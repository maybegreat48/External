#pragma once
#include "features.hpp"

namespace ext::program::vehicle
{
	inline void godmode(p p)
	{
		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.push(FALSE);
		p.native("GET_VEHICLE_PED_IS_IN"_n, 2, 1);
		p.local_load((uint16_t)features::vehicle_godmode);
		p.native("SET_ENTITY_INVINCIBLE"_n, 2, 0);
	}

	inline void target(p p)
	{
		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.push(FALSE);
		p.native("GET_VEHICLE_PED_IS_IN"_n, 2, 1);
		p.local_load((uint16_t)features::vehicle_targettable);
		p.native("SET_VEHICLE_CAN_BE_TARGETTED"_n, 2, 0);
	}

	inline void horn_boost(p p)
	{
		p.local_load((uint16_t)features::vehicle_horn_boost);
		p.jz("skip_horn_boost");

		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.push(FALSE);
		p.native("GET_VEHICLE_PED_IS_IN"_n, 2, 1);
		p.native("IS_HORN_ACTIVE"_n, 1, 1);
		p.jz("skip_horn_boost");

		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.push(FALSE);
		p.native("GET_VEHICLE_PED_IS_IN"_n, 2, 1);
		p.dup();

		p.native("GET_ENTITY_VELOCITY"_n, 1, 3);
		p.pushf(1.05f);
		p.f2v();
		p.vmul();
		p.push(0.5f);
		p.fsub();
		p.native("SET_ENTITY_VELOCITY"_n, 4, 0);

		p.label("skip_horn_boost");
	}

	inline void repair(p p)
	{
		p.local_load((uint16_t)features::vehicle_repair_queued);
		p.jz("skip_repair");

		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.push(FALSE);
		p.native("GET_VEHICLE_PED_IS_IN"_n, 2, 1);

		p.dup();
		p.dup();

		p.native("SET_VEHICLE_FIXED"_n, 1, 0);
		p.native("SET_VEHICLE_DEFORMATION_FIXED"_n, 1, 0);
		p.pushf(0.0f);
		p.native("SET_VEHICLE_DIRT_LEVEL"_n, 2, 0);

		p.push(FALSE);
		p.local_store((uint16_t)features::vehicle_repair_queued);

		p.label("skip_repair");
	}
}