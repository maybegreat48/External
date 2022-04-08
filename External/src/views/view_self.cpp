#include "views/view.hpp"
#include "core/globals.hpp"

namespace ext
{
	void view::self() {
		//components::button("Suicide", [] {
		//	ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0, 0);
		//});

		//ImGui::SameLine();

		//components::button("Skip Cutscene", [] {
		//	CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
		//});

		static char model[32];

		/*

		components::input_text_with_hint("Model Name###player_ped_model", "Player Model Name", model, sizeof(model), ImGuiInputTextFlags_EnterReturnsTrue, []
			{
				g_fiber_pool->queue_job([] {Hash hash = rage::joaat(model);

				for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
				{
					STREAMING::REQUEST_MODEL(hash);

					script::get_current()->yield();
				}
				if (!STREAMING::HAS_MODEL_LOADED(hash))
				{
					g_notification_service->push_error("Self", "Failed to spawn model, did you give an incorrect model ? ");

					return;
				}

				PLAYER::SET_PLAYER_MODEL(PLAYER::GET_PLAYER_INDEX(), hash);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				script::get_current()->yield();
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
				});
		});
		*/

		ImGui::Separator();

		ImGui::Text("General");

		ImGui::BeginGroup();

		components::feature_checkbox("God Mode", program::features::self_godmode, g->self.godmode);

		//ImGui::Checkbox("Off Radar", &g->self.off_radar);
		//ImGui::Checkbox("Free Cam", &g->self.free_cam);
		//ImGui::Checkbox("Disable Phone", &g->tunables.disable_phone);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		//ImGui::Checkbox("No Clip", &g->self.noclip);
		//ImGui::Checkbox("No Ragdoll", &g->self.no_ragdoll);
		components::feature_checkbox("Super Run", program::features::self_fast_run, g->self.super_run);
		//ImGui::Checkbox("No Idle Kick", &g->tunables.no_idle_kick);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		components::feature_checkbox("Invisibility", program::features::self_invisible, g->self.invisibility);
		//ImGui::Checkbox("Keep Player Clean", &g->self.clean_player);

		/*
		if (ImGui::Button("Clean Player"))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				entity::clean_ped(PLAYER::PLAYER_PED_ID());
			}
			QUEUE_JOB_END_CLAUSE
		}
		*/

		ImGui::EndGroup();

		ImGui::Separator();

		//components::small_text("Police");

		components::feature_checkbox("Never Wanted", program::features::self_no_cops, g->self.never_wanted);
	}
}