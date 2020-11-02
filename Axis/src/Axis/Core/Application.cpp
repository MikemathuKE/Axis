#include "axispch.h"
#include "Axis/Core/Application.h"

#include "Axis/Events/ApplicationEvent.h"
#include "Axis/Events/MouseEvent.h"
#include "Axis/Events/KeyEvent.h"
#include "Axis/Core/Log.h"

namespace Axis{

    Application::Application()
    {

    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        WindowResizeEvent e(1200, 720);
        printf("Here");
        AXIS_CORE_TRACE("{0}", e.ToString().c_str());
        if(e.IsInCategory(EventCategoryApplication)){
            printf("true");
        }

        while(true);
    }

}
