#include "gui/components/components.hpp"

namespace ext
{
	bool components::button(const std::string_view text) {
		return ImGui::Button(text.data());
	}

	void components::button(const std::string_view text, std::function<void()> cb) {
		if (components::button(text)) {
			cb();
		}
	}
}