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

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(AXIS_BIND_EVENT_FN(Application::OnWindowClose));

        AXIS_CORE_TRACE("{0}", e);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    } 

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}
