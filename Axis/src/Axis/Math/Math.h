#pragma once

#include <glm/glm.hpp>

namespace Axis { namespace Math {

	bool DecomposeTransform(const glm::mat4& Transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

}}