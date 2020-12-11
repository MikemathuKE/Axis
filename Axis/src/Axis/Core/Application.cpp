#include "axispch.h"
#include "Axis/Core/Application.h"

#include "Axis/Events/ApplicationEvent.h"
#include "Axis/Events/MouseEvent.h"
#include "Axis/Events/KeyEvent.h"
#include "Axis/Core/Log.h"

#include <GLFW/glfw3.h>

namespace Axis{

    Application::Application()
    {
        m_Window = (Scope<Window>)Window::Create();
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }

}
