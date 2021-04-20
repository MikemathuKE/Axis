#pragma once

#include "Camera.h"

namespace Axis {

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		virtual ~OrthographicCamera();

	protected:
		virtual void RecalculateViewMatrix() override;
	};

}