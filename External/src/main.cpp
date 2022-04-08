// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <natives.hpp>
#include <ysc.hpp>

#include "process.hpp"
#include "pattern.hpp"
#include "pointers.hpp"
#include "rage/natives.hpp"
#include "natives.hpp"
#include "script_loader.hpp"
#include "renderer.hpp"
#include "gui.hpp"

#include "services/gui_service.hpp"

using namespace ext;

inline uint32_t find_gta_proc_id() {
	auto win = ::FindWindowA("grcWindow", nullptr);
	if (!win) {
		LOG(FATAL) << "Cannot find game window";
	}

	DWORD a;
	::GetWindowThreadProcessId(win, &a);

	return a;
}

int main()
{
	try {
		std::filesystem::path base_dir = std::getenv("appdata");
		base_dir /= "External";

		auto file_manager_instance = std::make_unique<file_manager>(base_dir);

		auto logger_instance = std::make_unique<logger>(
			"External",
			file_manager_instance->get_project_file("./cout.log")
		);


		auto globals_instance = std::make_unique<menu_settings>(
			file_manager_instance->get_project_file("./settings.json")
		);

		LOG(INFO) << "External loaded.";

		auto process_instance = std::make_unique<process>(find_gta_proc_id());
		LOG(INFO) << "Process initalized.";

		auto pointers_instance = std::make_unique<pointers>();
		LOG(INFO) << "Pointers initialized.";

		auto script_loader_instance = std::make_unique<script_loader>();
		LOG(INFO) << "Script loader initialized.";

		g->load();
		LOG(INFO) << "Settings Loaded.";

		auto gui_service_instance = std::make_unique<gui_service>();
		LOG(INFO) << "Service instances registered";

		auto renderer_instance = std::make_unique<renderer>();
		LOG(INFO) << "Renderer initialized.";

		while (g_running) 
		{

			if (g_process->is_running())
			{
				g_renderer->on_tick();
				if (::GetKeyState(VK_F8) & 1)
				{
					g_renderer->toggle_opened();
				}
			}
			else {
				g_running = false;
			}
			::Sleep(0);
		}

		renderer_instance.reset();
		LOG(INFO) << "Renderer uninitialized.";

		script_loader_instance.reset();
		LOG(INFO) << "Script loader uninitialized.";

		pointers_instance.reset();
		LOG(INFO) << "Pointers uninitialized.";

		process_instance.reset();
		LOG(INFO) << "Process uninitialized.";


		LOG(INFO) << "Farewell!";

		globals_instance.reset();
		logger_instance.reset();
		file_manager_instance.reset();

		__fastfail(0);
	}
	catch (std::exception& e) {
		LOG(WARNING) << e.what();
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
