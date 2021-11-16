#pragma once

#include "Axis/Core/Layer.h"

namespace Axis {

	enum GUIBackend {
		ImGui,
		Nuklear
	};

	class GUILayer : public Layer
	{
	public:
		GUILayer(const char* layerName) : Layer(layerName) {}
		~GUILayer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void BlockEvents(bool block) = 0;

		static GUILayer* Create(GUIBackend gui = GUIBackend::Nuklear);
		static GUIBackend GetBackend();
	};

}