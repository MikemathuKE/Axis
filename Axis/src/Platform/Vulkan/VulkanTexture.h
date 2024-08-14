#pragma once

#include "Axis/Renderer/Texture.h"

#include <glad/glad.h>

namespace Axis {

    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        virtual void SetData(void* data, uint32_t size) override;

        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetRendererID() const override { return m_RendererID; }

        virtual void Bind(uint32_t slot = 0) const override;
        virtual bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
        }
    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };

    class OpenGLTextureCube : public TextureCube
    {
    public:
        OpenGLTextureCube(const std::vector<std::string>& filePaths);
        virtual ~OpenGLTextureCube();

        virtual uint32_t GetRendererID() const override { return m_RendererID; }
        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }

        virtual void SetData(void* data, uint32_t size) override {}

        virtual void Bind(uint32_t slot = 0) const override;
        virtual bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLTextureCube&)other).m_RendererID;
        }
    private:
        std::vector<std::string> m_FilePaths;
        uint32_t m_Width = 0, m_Height = 0;
        uint32_t m_RendererID;
        
    };
}
