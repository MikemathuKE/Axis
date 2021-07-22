#include "axispch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Axis {

	void Camera::RecalculateViewMatrix() {
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * rotate;

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}