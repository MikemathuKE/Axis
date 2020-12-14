#include "axispch.h"
#include "Axis/Core/Application.h"

#include "Axis/Events/MouseEvent.h"
#include "Axis/Events/KeyEvent.h"
#include "Axis/Core/Log.h"

#include <GLFW/glfw3.h>

namespace Axis{

    Application::Application()
    {
        m_Window = (Scope<Window>)Window::Create();
        m_Window->SetEventCallback(AXIS_BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application()
    {

    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(AXIS_BIND_EVENT_FN(Application::OnWindowClose));

        AXIS_CORE_TRACE("{0}", e);
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

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}
