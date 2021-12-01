#include "axispch.h"

#include "Axis/Renderer/Texture.h"

#include "Axis/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Axis/Core/Application.h"

namespace Axis {

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                AXIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLTexture2D>(width, height);
        }

        AXIS_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                AXIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLTexture2D>(path);
        }

        AXIS_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }

    Ref<TextureCube> TextureCube::Create(const std::vector<std::string>& facePaths)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            AXIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLTextureCube>(facePaths);
        }

        AXIS_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }
}
