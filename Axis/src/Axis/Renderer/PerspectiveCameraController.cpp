#include "axispch.h"

#include "Axis/Renderer/PerspectiveCameraController.h"
#include "Axis/Core/Input.h"
#include "Axis/Core/KeyCodes.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Axis {

	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float n, float f)
		:m_Camera(fov, aspectRatio, n, f)
	{
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		glm::vec3 cameraPosition = m_Camera.GetPosition();
		glm::vec3 cameraFront = m_Camera.GetDirection();
		glm::vec3 cameraUp = m_Camera.GetUp();

		if (Input::IsKeyPressed(AXIS_KEY_S))
			cameraPosition -= cameraFront * m_CameraTranslationSpeed * (float)ts;
		else if (Input::IsKeyPressed(AXIS_KEY_W))
			cameraPosition += cameraFront * m_CameraTranslationSpeed * (float)ts;

		if (Input::IsKeyPressed(AXIS_KEY_A))
			cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * m_CameraTranslationSpeed * (float)ts;
		else if (Input::IsKeyPressed(AXIS_KEY_D))
			cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * m_CameraTranslationSpeed * (float)ts;

		float newX = Input::GetMousePosX();
		float newY = Input::GetMousePosY();

		if (Input::IsMouseButtonPressed(AXIS_MOUSE_BUTTON_MIDDLE)) {
			m_Yaw += (newX - m_MouseX) * m_CameraRotationSpeed;
			m_Pitch += (newY - m_MouseY) * m_CameraRotationSpeed;

			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			else if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			direction.y = sin(glm::radians(m_Pitch));
			direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			cameraFront = glm::normalize(direction);

			m_Camera.SetDirection(cameraFront);
		}

		m_MouseX = newX;
		m_MouseY = newY;

		m_Camera.SetPosition(cameraPosition);
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(AXIS_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolledEvent));
	}

	bool PerspectiveCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{		
		float zoom = m_Camera.GetFieldOfView();
		printf("%f\n", zoom);
		zoom -= (float)e.GetYOffset() * m_CameraZoomSpeed;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 75.0f)
			zoom = 75.0f;
		
		m_Camera.SetFieldOfView(zoom);
		return false;
	}

}