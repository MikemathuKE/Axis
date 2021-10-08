#pragma once

#include <glm/glm.hpp>

namespace Axis {

	class Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;

		virtual const glm::mat4& GetViewProjectionMatrix() = 0;
	};

}