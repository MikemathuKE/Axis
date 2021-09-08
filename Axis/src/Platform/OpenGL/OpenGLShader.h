#ifndef OPENGL_SHADER_AXIS_H
#define OPENGL_SHADER_AXIS_H

/* OpenGL Shaders */

#include <Axis/Renderer/Shader.h>
#include <glm/glm.hpp>

//TODO remove
typedef unsigned int GLenum;

namespace Axis{

    class OpenGLShader : public Shader
    {
    public:
          OpenGLShader(const std::string& filePath);
          OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
          virtual ~OpenGLShader();

          virtual void Bind() const override;
          virtual void Unbind() const override;

          virtual void SetMat4(const std::string name, const glm::mat4& value) override;
          virtual void SetFloat4(const std::string name, const glm::vec4& value) override;
          virtual void SetFloat3(const std::string name, const glm::vec3& value) override;
          virtual void SetFloat(const std::string name, const float& value) override;
          virtual void SetInt(const std::string name, int value) override;


          virtual const std::string& GetName() const override {return m_Name;}

          //void UploadUniformInt(const std::string& name, int value);
          void UploadUniformInt(const std::string& name, const int& value);

          //void UploadUniformFloat(const std::string& name, float value);
          void UploadUniformFloat(const std::string& name, const float& value);
          void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
          void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
          void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

          void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
          void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    private:
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        std::string ReadFile(const std::string& filePath);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    private:
        uint32_t m_RendererID;
        std::string m_Name;
    };

}

#endif // OPENGL_SHADER_AXIS_H

