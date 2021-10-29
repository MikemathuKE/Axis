#pragma once

#include "Axis/Core/Core.h"

#include "Axis/Core/Window.h"
#include "Axis/Core/LayerStack.h"
#include "Axis/Events/Event.h"
#include "Axis/Events/ApplicationEvent.h"

#include "Axis/ImGui/ImGuiLayer.h"
#include "Axis/Nuklear/NuklearLayer.h"

namespace Axis{

    class Application
    {
    public:
        Application(const std::string& name = "Axis App");
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);
        void Close();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
        NuklearLayer* GetNuklearLayer() { return m_NuklearLayer; }

        inline static Application& Get() { return *s_Instance; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        NuklearLayer* m_NuklearLayer;

        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;

        float m_LastFrameTime = 0.0f;
    private:
        static Application* s_Instance;
    };

    Application* CreateApplication();

}
