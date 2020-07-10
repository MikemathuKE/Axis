#pragma once

/*
    Application Class defines the main Application abstraction.
*/

namespace Axis{

    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    //Function to be defined in client
    Application* CreateApplication();

}
