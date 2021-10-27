#include "axispch.h"

#include "Axis/GUI/GUI.h"
#include "Axis/GUI/Panel.h"
#include "Axis/GUI/Context.h"

namespace Axis {

	bool GUI::Begin(const char* title, const glm::vec2 pos)
	{
		PanelID panelId = PanelExists(title);
		if (panelId != GUI_INACTIVE) {
			s_GUIContext.CurrentPanel = panelId;
		}
		else {
			Panel panel;
			panel.Title = title;
			panel.Position = pos;
			panel.Id = s_GUIContext.PanelCount;

			s_GUIContext.Panels[panel.Id] = panel;
			s_GUIContext.CurrentPanel = panel.Id;
			s_GUIContext.PanelCount++;
		}
		return true;
	}

	PanelID GUI::PanelExists(const char* title)
	{
		PanelID panelId = GUI_INACTIVE;
		for (uint32_t i = 0; i < s_GUIContext.PanelCount; i++) {
			Panel panel = s_GUIContext.Panels[i];
			if (panel.Title == std::string(title)) {
				panelId = panel.Id;
				break;
			}
		}
		return panelId;
	}

	void GUI::End()
	{
		s_GUIContext.CurrentPanel = GUI_INACTIVE;
	}
}