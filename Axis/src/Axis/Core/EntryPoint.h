#pragma once

#if defined AXIS_PLATFORM_WINDOWS || defined AXIS_PLATFORM_LINUX

extern Axis::Application* Axis::CreateApplication();

int main(int32_t argc, char** argv)
{
    Axis::Log::Init();

    AXIS_PROFILE_BEGIN_SESSION("Startup", "AxisProfile-Startup.json");
    auto app = Axis::CreateApplication();
    AXIS_PROFILE_END_SESSION();

    AXIS_PROFILE_BEGIN_SESSION("Runtime", "AxisProfile-Runtime.json");
    app->Run();
    AXIS_PROFILE_END_SESSION();

    AXIS_PROFILE_BEGIN_SESSION("Shutdown", "AxisProfile-Shutown.json");
    delete app;
    AXIS_PROFILE_END_SESSION();
}

#endif // AXIS_PLATFORM_WINDOWS || AXIS_PLATFORM_LINUX
