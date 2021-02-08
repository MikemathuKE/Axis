#include "axispch.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Axis {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		AXIS_CORE_ASSERT(m_WindowHandle, "Window Handle is NULL!");
	}

	void OpenGLContext::Init()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwMakeContextCurrent(m_WindowHandle);
		int32_t status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AXIS_CORE_ASSERT(status, "Failed to initialize GLAD!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}