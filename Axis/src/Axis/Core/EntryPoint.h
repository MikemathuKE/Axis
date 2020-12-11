#pragma once

#if defined AXIS_PLATFORM_WINDOWS || defined AXIS_PLATFORM_LINUX

extern Axis::Application* Axis::CreateApplication();

int main(int32_t argc, char** argv)
{
    Axis::Log::Init();
    auto app = Axis::CreateApplication();
    app->Run();
    delete app;
}

#endif // AXIS_PLATFORM_WINDOWS || AXIS_PLATFORM_LINUX
