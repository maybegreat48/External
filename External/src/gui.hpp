#pragma once
#include "common.hpp"

namespace ext
{
	class gui
	{
	public:
		void dx_init();
		void dx_on_tick();
		void always_draw();

	public:
		bool m_opened{ true };
	};

	inline gui g_gui;
}