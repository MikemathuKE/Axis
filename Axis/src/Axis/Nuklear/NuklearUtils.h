#pragma once

// Include glm.hpp and nuklear.h before this header
#include <Nuklear/nuklear.h>
#include <glm/glm.hpp>

inline glm::vec4& operator<<(glm::vec4& vec, const nk_colorf& color)
{
	vec.r = color.r;
	vec.g = color.g;
	vec.b = color.b;
	vec.a = color.a;
	return vec;
}

inline nk_colorf& operator<<(nk_colorf& color, const glm::vec4& vec)
{
	color.r = vec.r;
	color.g = vec.g;
	color.b = vec.b;
	color.a = vec.a;
	return color;
}

namespace Axis {

	class Nuklear
	{
	public:
		static struct nk_context* GetContext();
	};
}