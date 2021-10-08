#pragma once

#include "Axis/Renderer/PerspectiveCamera.h"
#include "Axis/Core/Timestep.h"

#include "Axis/Events/ApplicationEvent.h"
#include "Axis/Events/MouseEvent.h"
#include "Axis/Events/KeyEvent.h"

namespace Axis {

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float fov, float aspectRatio, float n, float f);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
	private:
		PerspectiveCamera m_Camera;

		float m_Pitch = 0.0f;
		float m_Yaw = -90.0f;

		float m_MouseX = 0.0f;
		float m_MouseY = 0.0f;

		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 0.1f;
		float m_CameraZoomSpeed = 1.5f;
	};

}