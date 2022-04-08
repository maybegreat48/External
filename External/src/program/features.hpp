#pragma once
#include "ysc.hpp"
#include "natives.hpp"

namespace ext::program
{
	using p = ysc::program&;

	enum class features : std::uint16_t
	{
		self_godmode,
		self_invisible,
		self_fast_run,
		self_no_cops,

		weapons_infinite_ammo,
		weapons_infinite_clip,

		vehicle_godmode,
		vehicle_targettable,
		vehicle_horn_boost,
		vehicle_repair_queued,

		mobile_off_radar,

		spawn_in_vehicle,
		spawn_model,
		spawn_now,
		spawn_veh_handle, // as a temp variable
	};
}