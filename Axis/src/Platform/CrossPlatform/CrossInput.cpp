#include "axispch.h"

#include "Platform/CrossPlatform/CrossInput.h"

#include <GLFW/glfw3.h>
#include "Axis/Core/Application.h"

namespace Axis {

	bool CrossInput::IsKeyPressedImpl(KeyCode key)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool CrossInput::IsMouseButtonPressedImpl(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));

		return state == GLFW_PRESS;
	}

	std::pair<float, float> CrossInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}

	float CrossInput::GetMousePosXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float CrossInput::GetMousePosYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

}
