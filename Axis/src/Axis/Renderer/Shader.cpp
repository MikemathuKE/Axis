#include "axispch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Axis/Core/Application.h"

namespace Axis{

    Ref<Shader> Shader::Create(const std::string& filePath)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                AXIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLShader>(filePath);
        }

        AXIS_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                AXIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }

        AXIS_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }

    void ShaderLibrary::Add(const std::string&  name, const Ref<Shader>& shader)
    {
        AXIS_CORE_ASSERT(!Exists(name), "Shader Already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        auto& name = shader->GetName();
        Add(name, shader);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        AXIS_CORE_ASSERT(Exists(name), "Shader Not Found!");
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }

}
