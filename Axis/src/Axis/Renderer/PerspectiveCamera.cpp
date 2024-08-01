#include "axispch.h"
#include "Axis/Renderer/PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Axis {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float n, float f)
		: Camera(glm::perspective(glm::radians(fov), aspectRatio, n, f)), m_FOV(fov), m_AspectRatio(aspectRatio), m_Near(n), m_Far(f)
	{
		AXIS_PROFILE_FUNCTION();

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
		m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
	}

	PerspectiveCamera::~PerspectiveCamera()
	{}

	void PerspectiveCamera::RecalculateViewMatrix() {
		AXIS_PROFILE_FUNCTION();

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
		m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateProjectionMatrix() {
		AXIS_PROFILE_FUNCTION();

		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
		m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
	}

}