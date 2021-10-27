#pragma once

#include <vector>

#include "Axis/GUI/GUIDefs.h"
#include "Axis/GUI/Panel.h"
#include "Axis/GUI/Style.h"

namespace Axis {

	struct GUIContext {
		/* Panel */
		uint32_t PanelCount;
		PanelID ActivePanel = GUI_INACTIVE;
		PanelID CurrentPanel = GUI_INACTIVE;
		Panel Panels[MAX_PANELS];

		/* Style */
		GUIStyle Style;
	};
	static GUIContext s_GUIContext;

}