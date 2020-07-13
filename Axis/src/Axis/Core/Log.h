#pragma once

#include "Axis/Core/Core.h"

#include <spdlog/spdlog.h>

namespace Axis{

    class Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;}
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger;}
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

//Core Log macros
#define AXIS_CORE_TRACE(...) ::Axis::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AXIS_CORE_INFO(...)  ::Axis::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AXIS_CORE_WARN(...)  ::Axis::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AXIS_CORE_ERROR(...) ::Axis::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AXIS_CORE_FATAL(...) ::Axis::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client Log macros
#define AXIS_TRACE(...) ::Axis::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AXIS_INFO(...)  ::Axis::Log::GetClientLogger()->info(__VA_ARGS__)
#define AXIS_WARN(...)  ::Axis::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AXIS_ERROR(...) ::Axis::Log::GetClientLogger()->error(__VA_ARGS__)
#define AXIS_FATAL(...) ::Axis::Log::GetClientLogger()->fatal(__VA_ARGS__)
