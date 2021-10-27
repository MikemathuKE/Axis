#pragma once

#include "Axis/GUI/Context.h"
#include "Axis/GUI/Panel.h"

namespace Axis {

	class GUI {
	public:
		/* GUI */
		static void Init();
		static void Draw();

		/* Panel */
		static bool Begin(const char* title = "Window", const glm::vec2 pos = { 0.0f, 0.0f });
		static PanelID PanelExists(const char* title);
		static void End();

		/* Style */
		static void SetDefaultStyle(GUIContext& context);
	};

}