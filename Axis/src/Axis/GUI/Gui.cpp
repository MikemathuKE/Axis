#include "axispch.h"

#include "Axis/GUI/GUI.h"
#include "Axis/Renderer/Renderer2D.h"

namespace Axis {

	struct GuiRenderData {
	};
	static GuiRenderData s_GuiRenderData;

	void GUI::Init()
	{
		/* Panel */
		s_GUIContext.ActivePanel = GUI_INACTIVE;
		s_GUIContext.CurrentPanel = GUI_INACTIVE;
		s_GUIContext.PanelCount = 0;

		/* Style */
		SetDefaultStyle(s_GUIContext);
	}

	void GUI::Draw()
	{
		for (const auto& panel : s_GUIContext.Panels) {
			glm::vec2 border = { panel.Size.x + s_GUIContext.Style.BorderSize, panel.Size.y + s_GUIContext.Style.BorderSize };
			Renderer2D::DrawQuad(panel.Position, panel.Size, s_GUIContext.Style.Colors[PANEL_BACKGROUND_COLOR]);
			Renderer2D::DrawQuad(panel.Position, border, s_GUIContext.Style.Colors[GUI_BORDER_COLOR]);
		}
	}
}