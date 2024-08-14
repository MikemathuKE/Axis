#pragma once

/* OpenGL Shaders */

#include <Axis/Renderer/Shader.h>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

//TODO remove
typedef unsigned int GLenum;

namespace Axis{

    class VulkanShader : public Shader
    {
    public:
          VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, VkDevice& device);
          virtual ~VulkanShader();

          virtual void Bind() const override;
          virtual void Unbind() const override;

          virtual void SetMat4(const std::string name, const glm::mat4& value) override;
          virtual void SetFloat4(const std::string name, const glm::vec4& value) override;
          virtual void SetFloat3(const std::string name, const glm::vec3& value) override;
          virtual void SetFloat(const std::string name, const float& value) override;
          virtual void SetInt(const std::string name, int value) override;
          virtual void SetIntArray(const std::string name, int* values, int32_t count) override;

          virtual const std::string& GetName() const override {return m_Name;}
          std::vector<VkPipelineShaderStageCreateInfo> GetShaderStages() { return m_ShaderStages; }

          void UploadUniformInt(const std::string& name, const int& value);
          void UploadUniformIntArray(const std::string& name, int* values, int32_t count);

          void UploadUniformFloat(const std::string& name, const float& value);
          void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
          void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
          void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

          void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
          void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    private:
        std::vector<char> ReadFile(const std::string& filename);
        std::string Compile(const std::string& shaderSource);
        VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice& device);
    private:
        std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
        std::string m_Name;
    };

}

