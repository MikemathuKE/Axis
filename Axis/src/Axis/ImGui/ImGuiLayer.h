#pragma once

#include "Axis/Core/Layer.h"

namespace Axis {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnGUIRender() override;

		void Begin();
		void End();
	};

}