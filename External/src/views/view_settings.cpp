#include "views/view.hpp"

namespace ext
{
	void view::settings() {
		components::small_text("Settings");

		if (components::button("Save"))
		{
			g->save();
		}

		if (components::button("Load"))
		{
			g->load();
		}

		ImGui::PushItemWidth(350.f);

		ImGui::PopItemWidth();

		ImGui::Separator();

		components::small_text("Protections");

		ImGui::BeginGroup();
		ImGui::Checkbox("Bounty", &g->protections.script_events.bounty);
		ImGui::Checkbox("CEO Ban", &g->protections.script_events.ceo_ban);
		ImGui::Checkbox("CEO Kick", &g->protections.script_events.ceo_kick);
		ImGui::Checkbox("CEO Money", &g->protections.script_events.ceo_money);
		ImGui::Checkbox("Wanted Level", &g->protections.script_events.clear_wanted_level);
		ImGui::Checkbox("Fake Deposit", &g->protections.script_events.fake_deposit);
		ImGui::Checkbox("Force Mission", &g->protections.script_events.force_mission);
		ImGui::Checkbox("Force Teleport", &g->protections.script_events.force_teleport);
		ImGui::Checkbox("GTA Banner", &g->protections.script_events.gta_banner);
		ImGui::Checkbox("Network Bail", &g->protections.script_events.network_bail);
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Destroy Personal Vehicle", &g->protections.script_events.personal_vehicle_destroyed);
		ImGui::Checkbox("Remote Off Radar", &g->protections.script_events.remote_off_radar);
		ImGui::Checkbox("Rotate Cam", &g->protections.script_events.rotate_cam);
		ImGui::Checkbox("Send to Cutscene", &g->protections.script_events.send_to_cutscene);
		ImGui::Checkbox("Send to Island", &g->protections.script_events.send_to_island);
		ImGui::Checkbox("Sound Spam", &g->protections.script_events.sound_spam);
		ImGui::Checkbox("Spectate", &g->protections.script_events.spectate);
		ImGui::Checkbox("Transaction Error", &g->protections.script_events.transaction_error);
		ImGui::Checkbox("Vehicle Kick", &g->protections.script_events.vehicle_kick);
		ImGui::EndGroup();
	}
}