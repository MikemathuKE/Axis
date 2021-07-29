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
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }

        inline static Application& Get() { return *s_Instance; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);

    private:
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        NuklearLayer* m_NuklearLayer;

        bool m_Running = true;
        LayerStack m_LayerStack;

        float m_LastFrameTime = 0.0f;
    private:
        static Application* s_Instance;
    };

    Application* CreateApplication();

}
