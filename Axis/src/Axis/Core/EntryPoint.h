#pragma once

/*
    EntryPoint header file contains the start of the entire Program.
    It is included in the client side.
*/

#ifdef AXIS_PLATFORM_WINDOWS

extern Axis::Application* Axis::CreateApplication();

int main(int argc, char** argv)
{
    Axis::Log::Init();
    auto app = Axis::CreateApplication();
    app->Run();
    delete app;
}

#endif // AXIS_PLATFORM_WINDOWS
