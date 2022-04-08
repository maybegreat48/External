#include "components.hpp"
#include "script_loader.hpp"
#include "program/features.hpp"

namespace ext
{
	bool components::feature_checkbox(const std::string& text, program::features feature, bool& value)
	{
		if (ImGui::Checkbox(text.c_str(), &value))
		{
			g_script_loader->set_feature(feature, value);
			return true;
		}

		return false;
	}
}