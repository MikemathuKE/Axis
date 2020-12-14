#include "axispch.h"
#include "LinuxWindow.h"

#include "Axis/Events/ApplicationEvent.h"
#include "Axis/Events/KeyEvent.h"
#include "Axis/Events/MouseEvent.h"

namespace Axis {

    static uint8_t s_GLFWWindowCount = 0;
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		AXIS_CORE_ERROR("GLFW Error ({0}) : {1}", error, description);
	}

#ifdef AXIS_PLATFORM_LINUX
	Window* Window::Create(const WindowProps& props)
	{
		return new LinuxWindow(props);
	}
#endif // AXIS_PLATFORM_LINUX

	LinuxWindow::LinuxWindow(const WindowProps& props)
	{
		Init(props);
	}

	LinuxWindow::~LinuxWindow()
	{
		Shutdown();
	}

	void LinuxWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.Resizable = props.Resizable;

		AXIS_CORE_INFO("Creating Window {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);

		if (!s_GLFWInitialized)
		{
			//TODO ... GLFW Terminate on System Shutdown
			int32_t success = glfwInit();
			AXIS_CORE_ASSERT(success, "Could Not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_PrimaryMonitor = glfwGetPrimaryMonitor();
		m_VideoMode = *(glfwGetVideoMode(m_PrimaryMonitor));
		AXIS_CORE_TRACE("Storing underlying OS video mode: {0}x{1}@{2}Hz (r{3}g{4}b{5})",
			m_VideoMode.width,
			m_VideoMode.height,
			m_VideoMode.refreshRate,
			m_VideoMode.redBits,
			m_VideoMode.greenBits,
			m_VideoMode.blueBits);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, m_Data.Resizable);

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		m_OldWindowedParams.Width = props.Width;
		m_OldWindowedParams.Height = props.Height;
		glfwGetWindowPos(m_Window, &(m_OldWindowedParams.XPos), &(m_OldWindowedParams.YPos));
		SetWindowMode(props.Mode, 0, 0);

		//Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xPos, int yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowMovedEvent event(xPos, yPos);
			data.EventCallback(event);
		});

		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (focused)
			{
				case GLFW_TRUE:
				{
					WindowFocusedEvent event;
					data.EventCallback(event);
				}
				case GLFW_FALSE:
				{
					WindowFocusLostEvent event;
					data.EventCallback(event);
				}
			}
		});

		glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconified)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (iconified)
			{
				case GLFW_TRUE:
				{
					WindowIconifiedEvent event;
					data.EventCallback(event);
				}
				case GLFW_FALSE:
				{
					WindowRestoredEvent event;
					data.EventCallback(event);
				}
			}
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event((Axis::KeyCode)key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event((Axis::KeyCode)key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					// TODO: Allow Extracting repeat count
					KeyPressedEvent event((Axis::KeyCode)key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event((Axis::KeyCode)keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback( m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event((Axis::MouseCode)button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event((Axis::MouseCode)button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void LinuxWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void LinuxWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void LinuxWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	void LinuxWindow::SetWindowMode(const WindowMode& mode, unsigned int width, unsigned int height)
	{
		if (!m_Window)
			return;
		if (mode == m_Data.Mode)
			return;

		// If currently windowed, stash the current size and position of the window
		if (m_Data.Mode == WindowMode::WINDOWED)
		{
			m_OldWindowedParams.Width = m_Data.Width;
			m_OldWindowedParams.Height = m_Data.Height;
			glfwGetWindowPos(m_Window, &(m_OldWindowedParams.XPos), &(m_OldWindowedParams.YPos));
		}
		GLFWmonitor* monitor = nullptr;

		if (mode == WindowMode::BORDERLESS)
		{
			// For borderless full screen, the new width and height will be the video mode width and height
			width = m_VideoMode.width;
			height = m_VideoMode.height;
			monitor = m_PrimaryMonitor;
		}
		else if (mode == WindowMode::WINDOWED && (width == 0 || height == 0))
		{
			// For windowed, use old window height and width if none provided
			width = m_OldWindowedParams.Width;
			height = m_OldWindowedParams.Height;
			monitor = nullptr;
		}
		else if (mode == WindowMode::FULLSCREEN)
		{
			if (width == 0 || height == 0)
			{
				// Use the old window size
				// TODO: May want to change this to check if it is a valid full screen resolution pair
				width = m_Data.Width;
				height = m_Data.Height;
			}
			monitor = m_PrimaryMonitor;
		}

		// Update stored width and height
		m_Data.Width = width;
		m_Data.Height = height;

		// Trigger resize event
		if (m_Data.EventCallback)
		{
			WindowResizeEvent e(width, height);
			m_Data.EventCallback(e);
		}

		AXIS_CORE_TRACE("Changing window mode from {0} to {1}: [{2}, {3}]", ToString(m_Data.Mode), ToString(mode), width, height);

		// Record new window type
		m_Data.Mode = mode;

		glfwSetWindowMonitor(m_Window, monitor, m_OldWindowedParams.XPos, m_OldWindowedParams.YPos, width, height, m_VideoMode.refreshRate);
	}

	void LinuxWindow::SetIcon(const std::string& path)
	{
		//Requires stbi
		/*
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		AXIS_CORE_ASSERT(channels == 4, "Icon must be RGBA!");

		GLFWimage images[1];
		images[0].width = width;
		images[0].height = height;
		images[0].pixels = data;
		glfwSetWindowIcon(m_Window, 1, images);
		stbi_image_free(data);
		*/
	}

}
