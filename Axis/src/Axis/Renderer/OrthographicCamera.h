#pragma once

#include "Camera.h"

namespace Axis {

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		virtual ~OrthographicCamera();

		void SetRotation(float rotation) { m_Rotation = { 0.0f, 0.0f, rotation }; RecalculateViewMatrix(); }
	};

}