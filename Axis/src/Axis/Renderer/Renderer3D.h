#pragma once

#include <glm/glm.hpp>
#include "Axis/Renderer/PerspectiveCamera.h"

namespace Axis {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();
		static void Flush();
	};
}