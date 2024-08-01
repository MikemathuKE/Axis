#include "axispch.h"

#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image/stb_image.h>

#include "Axis/Core/Log.h"

namespace Axis
{

    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
        :m_Width(width), m_Height(height)
    {
        AXIS_PROFILE_FUNCTION();

        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        AXIS_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        :m_Path(path)
    {
        AXIS_PROFILE_FUNCTION();

        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        stbi_uc* data;
        {
            AXIS_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }

        AXIS_CORE_ASSERTs(data, "Failed to load image! {0}.", path.c_str());
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if(channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }else if(channels == 3){
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        AXIS_CORE_ASSERTs(internalFormat & dataFormat, "Image Format not supported! - ({0})", path.c_str());

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        AXIS_PROFILE_FUNCTION();

        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 :  3;
        AXIS_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        AXIS_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        AXIS_PROFILE_FUNCTION();

        glBindTextureUnit(slot, m_RendererID);
    }

    OpenGLTextureCube::OpenGLTextureCube(const std::vector<std::string>& filePaths)
        : m_FilePaths(filePaths)
    {
        AXIS_PROFILE_FUNCTION();

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
        int32_t width, height, channels;

        stbi_set_flip_vertically_on_load(false);
        for (unsigned int i = 0; i < filePaths.size(); i++)
        {
            auto data = stbi_load(filePaths[i].c_str(), &width, &height, &channels, 0);
            AXIS_CORE_ASSERTs(data, "Failed to load Image {0}", filePaths[i].c_str());
            
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            m_Width = (uint32_t)width;
            m_Height = (uint32_t)height;
        }        

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    OpenGLTextureCube::~OpenGLTextureCube()
    {
        AXIS_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTextureCube::Bind(uint32_t slot) const
    {
        AXIS_PROFILE_FUNCTION();

        glBindTextureUnit(slot, m_RendererID);
    }
}
