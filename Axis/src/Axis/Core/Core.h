#pragma once

#ifdef _WIN32
    #ifdef _WIN64
        #define AXIS_PLATFORM_WINDOWS
    #else
        #error "x32 builds not supported!"
    #endif // _WIN64
#elif defined(__linux__)
    #define AXIS_PLATFORM_LINUX
#else
    #error OS NOT SUPPORTED!
#endif // _WIN32

#ifdef _MSC_VER
    #define DEBUG_BREAK __debugbreak()
#else
    #define DEBUG_BREAK __builtin_trap
#endif // _MSC_VER

#ifdef AXIS_DEBUG
	#define AXIS_ASSERT(x, ...) { if(!(x)) { AXIS_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK; } }
	#define AXIS_CORE_ASSERT(x, ...) { if(!(x)) { AXIS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK; } }
#else
	#define AXIS_ASSERT(x, ...)
	#define AXIS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define AXIS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include <memory>

namespace Axis {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
