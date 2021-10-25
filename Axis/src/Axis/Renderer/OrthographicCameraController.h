#pragma once

#include "Axis/Renderer/OrthographicCamera.h"
#include "Axis/Core/Timestep.h"

#include "Axis/Events/ApplicationEvent.h"
#include "Axis/Events/MouseEvent.h"
#include "Axis/Events/KeyEvent.h"

#include <glm/glm.hpp>

namespace Axis {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; CalculateView(); }

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		void CalculateView();
		
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		float m_ZoomLevel = 1.0f;
		float m_AspectRatio;
		bool m_Rotation;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		float m_CameraRotation = 0.0f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 1.0f;
	};

}
