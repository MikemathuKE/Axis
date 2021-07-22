#include "axispch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Axis {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float n, float f)
		: Camera()
	{
		m_ProjectionMatrix = glm::perspective(fov, aspectRatio, n, f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	PerspectiveCamera::~PerspectiveCamera()
	{}

}