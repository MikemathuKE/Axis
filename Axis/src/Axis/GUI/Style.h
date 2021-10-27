#pragma once

#include <glm/glm.hpp>

#define PANEL_BACKGROUND_COLOR 0
#define GUI_BORDER_COLOR 1
#define MAX_STYLE_COLOR 2

namespace Axis {

	struct GUIStyle {
		glm::vec4 Colors[MAX_STYLE_COLOR];
		float BorderSize;
	};
}