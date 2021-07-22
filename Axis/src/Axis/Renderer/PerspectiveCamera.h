#pragma once

#include "Camera.h"

namespace Axis {

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio, float near, float far);
		virtual ~PerspectiveCamera();
	};

}