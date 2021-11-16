#include "axispch.h"

#include "Axis/Core/GUILayer.h"

#include "Axis/ImGui/ImGuiLayer.h"
#include "Axis/Nuklear/NuklearLayer.h"

namespace Axis {

	static GUIBackend s_GUIBackend;

	GUILayer* GUILayer::Create(GUIBackend gui)
	{
		s_GUIBackend = gui;
		switch (gui)
		{
		case Axis::ImGui: return new ImGuiLayer();
		case Axis::Nuklear: return new NuklearLayer();
		}
		AXIS_CORE_ASSERT(false, "GUI Backend not supported!");
		return nullptr;
	}

	GUIBackend GUILayer::GetBackend()
	{
		return s_GUIBackend;
	}

}