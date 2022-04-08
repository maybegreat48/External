#pragma once
#include "features.hpp"

namespace ext::program::spawn
{
	inline void loop(p p)
	{
		p.local_load((uint16_t)features::spawn_now);
		p.jz("skip_spawn");

		p.local_load((uint16_t)features::spawn_model);
		p.native("IS_MODEL_IN_CDIMAGE"_n, 1, 1);
		p.jz("done_spawn");

		p.local_load((uint16_t)features::spawn_model);
		p.dup();
		p.native("REQUEST_MODEL"_n, 1, 0);

		p.native("HAS_MODEL_LOADED"_n, 1, 1);
		p.jz("skip_spawn");

		p.local_load((uint16_t)features::spawn_model); // modelHash
		p.native("PLAYER_ID"_n, 0, 1);
		p.push(FALSE);
		p.native("GET_ENTITY_COORDS"_n, 2, 3); // x, y, z
		p.pushf(0.0f); // heading
		p.push(TRUE); // isNetwork
		p.push(FALSE); // bScriptHostVeh
		p.push(FALSE); // p7

		p.native("CREATE_VEHICLE"_n, 8, 1);

		p.local_load((uint16_t)features::spawn_model); // modelHash
		p.native("SET_MODEL_AS_NO_LONGER_NEEDED"_n, 1, 0);

		p.dup();
		p.local_store((uint16_t)features::spawn_veh_handle);
		p.jz("done_spawn"); // failed

		p.local_load((uint16_t)features::spawn_veh_handle);
		p.dup();
		p.dup();
		p.push(TRUE);
		p.native("_SET_ENTITY_CLEANUP_BY_ENGINE"_n, 2, 0);

		p.push(FALSE);
		p.native("SET_VEHICLE_IS_STOLEN"_n, 2, 0);

		p.native("VEH_TO_NET"_n, 1, 1);
		p.push(TRUE);
		p.native("SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES"_n, 2, 0);

		p.local_load((uint16_t)features::spawn_in_vehicle);
		p.jz("done_spawn");

		p.native("PLAYER_PED_ID"_n, 0, 1);
		p.local_load((uint16_t)features::spawn_veh_handle);
		p.push(-1);
		p.native("SET_PED_INTO_VEHICLE"_n, 3, 0);

		p.label("done_spawn");
		p.push(FALSE);
		p.local_store((uint16_t)features::spawn_now);

		p.label("skip_spawn");
	}
}