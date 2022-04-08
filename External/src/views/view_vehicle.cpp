#include "views/view.hpp"

namespace ext
{
	void view::vehicle() {
		ImGui::BeginGroup();
		components::feature_checkbox("Can Be Targeted", program::features::vehicle_targettable, g->vehicle.is_targetable);
		components::feature_checkbox("God Mode", program::features::vehicle_godmode, g->vehicle.god_mode);
		components::feature_checkbox("Horn Boost", program::features::vehicle_horn_boost, g->vehicle.horn_boost);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		if (components::button("Repair"))
		{
			g_script_loader->set_feature(program::features::vehicle_repair_queued, true);
		}
#if 0

		if (components::button("Handling")) {
			ImGui::OpenPopup("Handling Popup");
		}

		bool enabled = true;

		ImGui::SetNextWindowSize({ (float)g->window.x * 0.5f, (float)g->window.y * 0.5f }, ImGuiCond_FirstUseEver);
		if (ImGui::BeginPopupModal("Handling Popup", &enabled, ImGuiWindowFlags_MenuBar))
		{
			if (g_local_player == nullptr || g_local_player->m_vehicle == nullptr || g_local_player->m_ped_task_flag & (int)ePedTask::TASK_FOOT)
			{
				ImGui::Text("Please enter a vehicle.");
				ImGui::EndPopup();
				return;
			}
			g_vehicle_service->attempt_save();

			ImGui::BeginTabBar("handling_profiles");
			tab_handling::tab_current_profile();
			tab_handling::tab_my_profiles();
			tab_handling::tab_saved_profiles();
			tab_handling::tab_search();
			ImGui::EndTabBar();

			ImGui::EndPopup();
		}
#endif

		ImGui::EndGroup();
#if 0

		ImGui::Separator();

		components::small_text("LS Customs");

		components::button("Start LS Customs", [] {
			g->vehicle.ls_customs = true;
			});

		ImGui::Separator();

		components::small_text("Speedo Meter");

		SpeedoMeter selected = g->vehicle.speedo_meter.type;

		ImGui::Text("Type:");
		if (ImGui::BeginCombo("###speedo_type", speedo_meters[(int)selected].name))
		{
			for (const speedo_meter& speedo : speedo_meters)
			{
				if (ImGui::Selectable(speedo.name, speedo.id == selected))
				{
					g->vehicle.speedo_meter.type = speedo.id;
				}

				if (speedo.id == selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();

		}

		ImGui::Text("Position");

		float pos[2];
		pos[0] = g->vehicle.speedo_meter.x;
		pos[1] = g->vehicle.speedo_meter.y;
		if (ImGui::SliderFloat2("###speedo_pos", pos, .001f, .999f, "%.3f"))
		{
			g->vehicle.speedo_meter.x = pos[0];
			g->vehicle.speedo_meter.y = pos[1];
		}

		ImGui::Checkbox("Left Sided", &g->vehicle.speedo_meter.left_side);

#endif
	}
}