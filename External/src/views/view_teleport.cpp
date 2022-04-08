#include "views/view.hpp"

namespace ext
{
	void view::teleport() {
#if 0
		ImGui::Text("Blips:");

		components::button("Waypoint", [] {
			teleport::to_waypoint();
			});

		components::button("Objective", [] {
			static const int blips[] = { 1, 57, 128, 129, 130, 143, 144, 145, 146, 271, 286, 287, 288 };
			for (int i = 0; i < (sizeof(blips) / sizeof(*blips)); i++) {
				if (teleport::to_blip(blips[i], 5)) {
					break;
				}

			}
			});

		ImGui::Text("Vehicles:");

		components::button("Bring Personal Vehicle", [] {
			Vehicle veh = globals::get_personal_vehicle();
			if (ENTITY::IS_ENTITY_DEAD(veh, false)) return g_notification_service->push_error("Teleport", "Invalid vehicle handle...");

			Vector3 location = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);

			vehicle::bring(veh, location);
			});

		components::button("Teleport to Personal Vehicle", [] {
			Vehicle veh = globals::get_personal_vehicle();
			if (ENTITY::IS_ENTITY_DEAD(veh, false)) return g_notification_service->push_error("Teleport", "Invalid vehicle handle...");

			teleport::to_coords(
				ENTITY::GET_ENTITY_COORDS(veh, true)
			);
			});
#endif
	}
}