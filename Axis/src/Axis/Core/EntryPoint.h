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
    AXIS_CORE_INFO("{0}", argv[0]);
    AXIS_CORE_TRACE("LOG!");
    AXIS_CORE_WARN("LOG!");
    int a = 5;
    AXIS_ERROR("LOG - Var={0}", a);

    auto app = Axis::CreateApplication();
    app->Run();
    delete app;
}

#endif // AXIS_PLATFORM_WINDOWS
