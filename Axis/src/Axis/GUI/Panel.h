#pragma once

#include <glm/glm.hpp>

#include "Axis/GUI/GUIDefs.h"
#include <string>

namespace Axis {

	struct Panel
	{
		glm::vec2 Position = { 0, 0 };
		glm::vec2 Size = { MAX_PANEL_WIDTH, MAX_PANEL_HEIGHT };
		glm::vec2 MaxSize = { MAX_PANEL_WIDTH, MAX_PANEL_HEIGHT };
		std::string Title;
		PanelID Id;

		bool operator==(const Panel& otherPanel) {
			return this->Id == otherPanel.Id;
		}
	};
}