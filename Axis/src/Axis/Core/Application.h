#pragma once

#include "Window.h"

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

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        Scope<Window> m_Window;
        bool m_Running = true;
    };

    Application* CreateApplication();

}
