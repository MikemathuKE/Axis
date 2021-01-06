#pragma once

#include "Axis/Core/Layer.h"

#include "Axis/Events/ApplicationEvent.h"
#include "Axis/Events/KeyEvent.h"
#include "Axis/Events/MouseEvent.h"

namespace Axis {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}