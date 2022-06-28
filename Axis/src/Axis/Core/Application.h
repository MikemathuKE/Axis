#pragma once

#include "Axis/Core/Base.h"

#include "Axis/Core/Window.h"
#include "Axis/Core/LayerStack.h"
#include "Axis/Events/Event.h"
#include "Axis/Events/ApplicationEvent.h"

#include "Axis/Core/GUILayer.h"

namespace Axis{

    class Application
    {
    public:
        Application(const std::string& name = "Axis App", GUIBackend backend = GUIBackend::Nuklear);
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);
        void Close();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }

        GUILayer* GetGUILayer() { return m_GUILayer; }

        inline static Application& Get() { return *s_Instance; }

        float GetDeltaTime();
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        Scope<Window> m_Window;
        GUILayer* m_GUILayer;

        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;

        float m_LastFrameTime = 0.0f;
    private:
        static Application* s_Instance;
    };

    Application* CreateApplication();

}
