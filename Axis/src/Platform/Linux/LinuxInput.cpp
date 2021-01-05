#include "axispch.h"

#include "Platform/Linux/LinuxInput.h"

#include <GLFW/glfw3.h>
#include "Axis/Core/Application.h"

namespace Axis {

	bool LinuxInput::IsKeyPressedImpl(KeyCode key)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool LinuxInput::IsMouseButtonPressedImpl(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));

		return state == GLFW_PRESS;
	}

	std::pair<float, float> LinuxInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}

	float LinuxInput::GetMousePosXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float LinuxInput::GetMousePosYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

}
