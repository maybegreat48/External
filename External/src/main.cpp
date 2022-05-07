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

using namespace ext;

inline std::uint32_t find_gta_proc_id() 
{
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
	std::filesystem::path base_dir = std::getenv("appdata");
	base_dir /= "External";

	auto file_manager_instance = std::make_unique<file_manager>(base_dir);

	auto logger_instance = std::make_unique<logger>(
		"External",
		file_manager_instance->get_project_file("./cout.log")
	);

	LOG(INFO) << "External loaded.";

	auto process_instance = std::make_unique<process>(find_gta_proc_id());
	LOG(INFO) << "Process initalized.";

	auto pointers_instance = std::make_unique<pointers>();
	LOG(INFO) << "Pointers initialized.";

	auto script_loader_instance = std::make_unique<script_loader>();
	LOG(INFO) << "Script loader initialized.";

	__fastfail(0);

	while (g_running && g_process->is_running()) 
	{
		::Sleep(100);
	}

	script_loader_instance.reset();
	LOG(INFO) << "Script loader uninitialized.";

	pointers_instance.reset();
	LOG(INFO) << "Pointers uninitialized.";

	process_instance.reset();
	LOG(INFO) << "Process uninitialized.";

	LOG(INFO) << "Farewell!";

	logger_instance.reset();
	file_manager_instance.reset();

	return 0;
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
