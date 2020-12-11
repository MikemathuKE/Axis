#pragma once

#include "Window.h"

namespace Axis{

    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        Scope<Window> m_Window;
        bool m_Running = true;
    };

    Application* CreateApplication();

}
