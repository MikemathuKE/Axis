#include "axispch.h"

#ifdef AXIS_PLATFORM_LINUX
    #define STBI_NO_SIMD
#endif // AXIS_PLATFORM_LINUX

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
