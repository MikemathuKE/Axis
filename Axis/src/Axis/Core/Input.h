#pragma once

#include "Axis/Core/Core.h"
#include "Axis/Core/KeyCodes.h"
#include "Axis/Core/MouseCodes.h"

namespace Axis {

	class Input
	{
	public:
		inline static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }
		inline static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMousePosX() { return s_Instance->GetMousePosXImpl(); }
		inline static float GetMousePosY() { return s_Instance->GetMousePosYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(KeyCode key) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMousePosXImpl() = 0;
		virtual float GetMousePosYImpl() = 0;

	private:
		static Scope<Input> s_Instance;
	};
}

