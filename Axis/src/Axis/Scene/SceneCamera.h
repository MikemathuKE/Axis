#pragma once

#include "Axis/Renderer/Camera.h"

namespace Axis {

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float fov, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float orthographicSize) 
			{ m_OrthographicSize = orthographicSize;  RecalculateProjection(); }

		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float fov)
			{ m_PerspectiveFOV = fov; RecalculateProjection(); }

		void SetOrthographic(bool orthographic)
			{ m_IsOrthographic = orthographic; RecalculateProjection(); }
		void SetPerspective(bool perspective)
			{ m_IsOrthographic = perspective; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_PerspectiveFOV = 45.0f;
		float m_PerspectiveNear = -2.0f, m_PerspectiveFar = 100.0f;
		
		float m_AspectRatio = 0.0f;
		bool m_IsOrthographic = true;
	};

}
