#pragma once

#include "Axis/Core/Core.h"
#include "Axis/Core/KeyCodes.h"
#include "Axis/Core/MouseCodes.h"

namespace Axis {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMousePosX();
		static float GetMousePosY();
	};
}

