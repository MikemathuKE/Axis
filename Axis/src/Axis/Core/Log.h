#pragma once

#include "Axis/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Axis{

    class Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;}
        static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger;}
    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;

    };

}

template<typename... Args>
static std::string CreateFormatType(const char* str, const int& line, const char* file) 
{
    std::string buffer;
    buffer = std::string(str);
    buffer += std::string(" (:"); 
    buffer += std::to_string(line);
    buffer += std::string(") ");
    buffer += std::string(" ... ");
    buffer += std::string(file);
    return buffer;
}

//Core Log macros
#define AXIS_CORE_TRACE_MAIN(...) ::Axis::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AXIS_CORE_TRACE(str, ...) AXIS_CORE_TRACE_MAIN(CreateFormatType(str, __LINE__, __FILE__).c_str(), __VA_ARGS__)
#define AXIS_CORE_INFO_MAIN(...)  ::Axis::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AXIS_CORE_INFO(str, ...) AXIS_CORE_INFO_MAIN(CreateFormatType(str, __LINE__, __FILE__).c_str(), __VA_ARGS__)
#define AXIS_CORE_WARN_MAIN(...)  ::Axis::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AXIS_CORE_WARN(str, ...) AXIS_CORE_WARN_MAIN(CreateFormatType(str, __LINE__, __FILE__).c_str(), __VA_ARGS__)
#define AXIS_CORE_ERROR_MAIN(...) ::Axis::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AXIS_CORE_ERROR(str, ...) AXIS_CORE_ERROR_MAIN(CreateFormatType(str, __LINE__, __FILE__).c_str(), __VA_ARGS__)
#define AXIS_CORE_FATAL_MAIN(...) ::Axis::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define AXIS_CORE_FATAL(str, ...) AXIS_CORE_FATAL_MAIN(CreateFormatType(str, __LINE__, __FILE__).c_str(), __VA_ARGS__)

//Client Log macros
#define AXIS_TRACE_MAIN(...) ::Axis::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AXIS_TRACE(...) AXIS_TRACE_MAIN(CreateFormatType(str, __LINE__, __FILE__).c_str(), __VA_ARGS__)
#define AXIS_INFO_MAIN(...)  ::Axis::Log::GetClientLogger()->info(__VA_ARGS__)
#define AXIS_INFO(...) AXIS_INFO_MAIN(CreateFormatType(str, __LINE__, __FILE__).c_str(), __VA_ARGS__)
#define AXIS_WARN_MAIN(...)  ::Axis::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AXIS_WARN(...) AXIS_WARN_MAIN(CreateFormatType(str, __LINE__, __FILE__).c_str(), __VA_ARGS__)
#define AXIS_ERROR_MAIN(...) ::Axis::Log::GetClientLogger()->error(__VA_ARGS__)
#define AXIS_ERROR(...) AXIS_ERROR_MAIN(CreateFormatType(str, __LINE__, __FILE__).c_str(), __VA_ARGS__)
#define AXIS_FATAL_MAIN(...) ::Axis::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define AXIS_FATAL(...) AXIS_FATAL_MAIN(CreateFormatType(str, __LINE__, __FILE__).c_str(), __VA_ARGS__)
