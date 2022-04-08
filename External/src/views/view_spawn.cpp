#include "views/view.hpp"
#include "natives.hpp"
#include "core/globals.hpp"
#include "rage/joaat.hpp"

namespace ext
{
	static char model[24] = "";

	bool does_search_match(std::string& input, const std::string& search)
	{
		std::transform(input.begin(), input.end(), input.begin(), ::tolower);

		return input.find(search) != std::string::npos;
	}

	void view::spawn() {
		//ImGui::Checkbox("Preview", &g->spawn.preview_vehicle);
		//ImGui::SameLine();
		components::feature_checkbox("Spawn Inside", program::features::spawn_in_vehicle, g->spawn.spawn_inside);
		components::input_text_with_hint("Model Name", "(enter to spawn)", model, sizeof(model), ImGuiInputTextFlags_EnterReturnsTrue, []
		{
			if (!g_script_loader->get_feature<bool>(program::features::spawn_now))
			{
				g_script_loader->set_feature(program::features::spawn_model, rage::joaat("oppressor2"));
				g_script_loader->set_feature(program::features::spawn_now, true);
			}
		});
		
#if 0
		if (ImGui::ListBoxHeader("###vehicles", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() }))
		{
			if (!g_vehicle_preview_service->get_vehicle_list().is_null())
			{
				for (auto& item : g_vehicle_preview_service->get_vehicle_list())
				{
					if (item["Name"].is_null() || item["DisplayName"].is_null())
						continue;

					std::string name = item["Name"];
					std::string display_name = item["DisplayName"];

					std::string manufacturer;
					std::string search = model;
					std::transform(search.begin(), search.end(), search.begin(), ::tolower);

					if (!item["ManufacturerDisplayName"].is_null())
						manufacturer = item["ManufacturerDisplayName"];

					if (search.empty() ||
						does_search_match(name, search) ||
						does_search_match(display_name, search) ||
						does_search_match(manufacturer, search))
					{
						components::selectable(item["DisplayName"], item["Name"] == search, [&item]
						{
							const auto location = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
							const Vehicle veh = vehicle::spawn(item["Name"], location, 0.f);

							if (g->spawn.spawn_inside)
								PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
						});

						if (g->spawn.preview_vehicle && ImGui::IsItemHovered())
							g_vehicle_preview_service->set_preview_vehicle(item);
					}
				}
			}
			else ImGui::Text("No vehicles in registry.");
		}
#endif
	}
}