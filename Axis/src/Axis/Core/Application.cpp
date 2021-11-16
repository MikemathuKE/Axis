#include "axispch.h"
#include "Axis/Core/Application.h"

#include "Axis/Events/MouseEvent.h"
#include "Axis/Events/KeyEvent.h"
#include "Axis/Core/Log.h"

#include "Axis/Core/Input.h"
#include "Axis/Renderer/Renderer.h"

#include "Axis/Core/Timestep.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Axis{

    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name)
    {
        AXIS_PROFILE_FUNCTION();
        
        AXIS_CORE_ASSERT(!s_Instance, "Application already Exists!");
        s_Instance = this;

        m_Window = (Scope<Window>)Window::Create(WindowProps(name));
        m_Window->SetEventCallback(AXIS_BIND_EVENT_FN(Application::OnEvent));

        m_GUILayer = GUILayer::Create(GUIBackend::Nuklear);
        PushOverlay(m_GUILayer);

        Renderer::Init();
        m_Window->SetVSync(true);
    }

    Application::~Application()
    {
        Renderer::Shutdown();
    }

    void Application::PushLayer(Layer* layer)
    {
        AXIS_PROFILE_FUNCTION();

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        AXIS_PROFILE_FUNCTION();

        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        AXIS_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(AXIS_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(AXIS_BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        AXIS_PROFILE_FUNCTION();

        while (m_Running)
        {
            AXIS_PROFILE_SCOPE("RunLoop");

            float time = (float)glfwGetTime();
            Timestep ts = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized) {
                {
                    AXIS_PROFILE_SCOPE("LayerStack OnUpdate");
                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(ts);
                }
                
                m_GUILayer->Begin();
                {
                    AXIS_PROFILE_SCOPE("GUI Rendering");
                    for (Layer* layer : m_LayerStack)
                        layer->OnGUIRender();
                }
                m_GUILayer->End();
                
            }
            


            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    void Application::Close()
    {
        m_Running = false;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        AXIS_PROFILE_FUNCTION();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }

}
