
#include "axispch.h"

#include "Axis/utils/PlatformUtils.h"

#ifdef AXIS_PLATFORM_LINUX

#include <GLFW/glfw3.h>
// #define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Axis/Core/Application.h"

namespace Axis {

	std::string FileDialogs::OpenFile(const char* filter)
	{
        return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
        return std::string();
	}

}

#endif // AXIS_PLATFORM_LINUX

