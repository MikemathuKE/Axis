#pragma once

#include <memory>

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
#endif // AXIS_PLATFORM_WINDOWS

#define BIT(x) (1 << x)

#define AXIS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

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
