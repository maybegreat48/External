#pragma once
#include "imgui.h"
#include "natives.hpp"
#include "pointers.hpp"
#include "gui/components/components.hpp"
#include "core/globals.hpp"

namespace ext
{
	class view
	{
		//inline static animator window_animator = animator();
		inline static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav;

	public:
		static void active_view();
		static void debug();
		static void esp_settings();
		static void heading();
		static void mobile();
		static void navigation();
		//static void player_navigation();
		static void notifications();
		static void root();
		static void self();
		static void session();
		static void settings();
		static void spawn();
		static void spoofing();
		static void teleport();
		static void vehicle();
		static void view_player();
		static void players();
		static void weapons();

		static void always()
		{
			//esp::draw();
			//notifications();
		}
	};
}