#pragma once

#include <glm/glm.hpp>

namespace Axis {

	class Camera
	{
	public:
		Camera() :m_ViewMatrix(1.0f) {}
		virtual ~Camera() {}

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix();  }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		void SetRotationX(float rotation) { m_Rotation = { rotation, 0.0f, 0.0f }; RecalculateViewMatrix();  }
		void SetRotationY(float rotation) { m_Rotation = { 0.0f, rotation, 0.0f }; RecalculateViewMatrix(); }
		void SetRotationZ(float rotation) { m_Rotation = { 0.0f, 0.0f, rotation }; RecalculateViewMatrix(); }

		const glm::vec3& GetPosition() { return m_Position; }
		const glm::vec3& GetRotation() { return m_Rotation; }

		const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }
	protected:
		void RecalculateViewMatrix();
	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	};

}