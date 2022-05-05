#pragma once

#define DISABLE_VECTORED_EXCEPTIONHANDLING

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <chrono>
#include <list>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <thread>

#include <Windows.h>

#include <Psapi.h>

#pragma comment(lib, "psapi.lib")

#include "logger.hpp"

namespace std
{
	template <class T, template <class...> class Template>
	struct is_specialization : std::false_type {};

	template <template <class...> class Template, class... Args>
	struct is_specialization<Template<Args...>, Template> : std::true_type {};
}

namespace ext 
{
	using namespace std::chrono_literals;

	inline std::atomic_bool g_running{ true };
}