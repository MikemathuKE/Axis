#pragma once

#include "Axis/Core/Core.h"

#include "Axis/Core/Window.h"
#include "Axis/Core/LayerStack.h"
#include "Axis/Events/Event.h"
#include "Axis/Events/ApplicationEvent.h"

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
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        Scope<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
    };

    Application* CreateApplication();

}
