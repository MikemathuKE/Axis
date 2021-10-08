#pragma once

#include "Axis/Renderer/Camera.h"

namespace Axis {

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);
		virtual ~OrthographicCamera();

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const float GetRotation() { return m_Rotation; }
	private:
		void RecalculateViewMatrix();
	private:
		float m_Rotation;
	};

}