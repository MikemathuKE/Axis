#include "axispch.h"

#include "Axis/GUI/Style.h"
#include "Axis/GUI/GUI.h"

namespace Axis {

	void GUI::SetDefaultStyle(GUIContext& context) {
		/* Colors */
		context.Style.Colors[PANEL_BACKGROUND_COLOR] = { 0.0f, 0.0f, 0.0f, 1.0f };
		context.Style.Colors[GUI_BORDER_COLOR] = { 0.3f, 0.3f, 0.3f, 1.0f };

		/* Border */
		context.Style.BorderSize = 0.1f;
	}

}