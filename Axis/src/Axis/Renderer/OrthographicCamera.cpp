#include "axispch.h"
#include "Axis/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Axis {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:Camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
	{
		AXIS_PROFILE_FUNCTION();
		m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		AXIS_PROFILE_FUNCTION();

		m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
	}

	OrthographicCamera::~OrthographicCamera()
	{
	}

	void OrthographicCamera::RecalculateViewMatrix() {
		AXIS_PROFILE_FUNCTION();

		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * rotate;

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
	}

}