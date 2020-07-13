#pragma once

#ifdef _WIN32
    #ifdef _WIN64
        #define AXIS_PLATFORM_WINDOWS
    #else
        #error "x32 builds not supported!"
    #endif // _WIN64
#else
    #error Axis Only Supports Windows!
#endif // AXIS_PLATFORM_WINDOWS
