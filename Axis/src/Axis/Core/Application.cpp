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

    Application::Application()
    {
        AXIS_CORE_ASSERT(!s_Instance, "Application already Exists!");
        s_Instance = this;

        m_Window = (Scope<Window>)Window::Create();
        m_Window->SetEventCallback(AXIS_BIND_EVENT_FN(Application::OnEvent));
        m_Window->SetIcon("assets/textures/AxisLogo.png");

        m_ImGuiLayer = new ImGuiLayer();
        PushLayer(m_ImGuiLayer);
        m_NuklearLayer = new NuklearLayer();
        PushLayer(m_NuklearLayer);

        Renderer::Init();
        m_Window->SetVSync(true);
    }

    Application::~Application()
    {}

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
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
        while (m_Running)
        {
            float time = (float)glfwGetTime();
            Timestep ts = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized) {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(ts);

            }
            m_ImGuiLayer->Begin();
            m_NuklearLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnGUIRender();
            m_NuklearLayer->End();
            m_ImGuiLayer->End();


            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
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
