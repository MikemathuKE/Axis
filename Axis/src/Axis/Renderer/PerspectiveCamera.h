#pragma once

#include "Axis/Renderer/Camera.h"

namespace Axis {

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio, float near, float far);
		virtual ~PerspectiveCamera();

		glm::vec3 GetPosition() { return m_Position; }
		glm::vec3 GetDirection() { return m_Direction; }
		glm::vec3 GetUp() { return m_Up; }
		float GetFieldOfView() { return m_FOV; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetDirection(const glm::vec3& front) { m_Direction = front; RecalculateViewMatrix(); }
		void SetUp(const glm::vec3& up) { m_Up = up; RecalculateViewMatrix(); }
		void SetFieldOfView(float fov) { m_FOV = fov; RecalculateProjectionMatrix(); }
	private:
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();
	private:
		glm::vec3 m_Direction = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };

		float m_FOV;
		float m_AspectRatio;
		float m_Near;
		float m_Far;
	};

}