#include "axispch.h"
#include "Platform/Vulkan/VulkanShader.h"

#include "Axis/Core/Log.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>

namespace Axis{

    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if(type == "vertex")
            return VK_SHADER_STAGE_VERTEX_BIT;
        if(type == "fragment" || type == "pixel")
            return VK_SHADER_STAGE_FRAGMENT_BIT;

        AXIS_CORE_ASSERT(false, "Unknown Shader type!");
        return 0;
    }

    static std::string ShaderTypeToString(const GLenum type)
    {
        if (type == VK_SHADER_STAGE_VERTEX_BIT)
            return "Vertex";
        if (type == VK_SHADER_STAGE_FRAGMENT_BIT)
            return "Fragment";

        AXIS_CORE_ASSERT(false, "Unknown Shader type!");
        return 0;
    }

    VulkanShader::VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, VkDevice& device)
        :m_Name(name)
    {
        AXIS_PROFILE_FUNCTION();

        auto vertShaderCode = ReadFile(vertexSrc);
        auto fragShaderCode = ReadFile(fragmentSrc);

        VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode, device);
        VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode, device);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";
        vertShaderStageInfo.pSpecializationInfo = nullptr; // Optional // Used for constants

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        m_ShaderStages = { vertShaderStageInfo, fragShaderStageInfo };
    }

    VulkanShader::~VulkanShader()
    {
        AXIS_PROFILE_FUNCTION();

        
    }

    VkShaderModule VulkanShader::CreateShaderModule(const std::vector<char>& code, VkDevice& device) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        AXIS_CORE_ASSERT(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS, "Failed to Create Shader Module!");

        return shaderModule;
    }

    std::vector<char> VulkanShader::ReadFile(const std::string& filename) {
        AXIS_PROFILE_FUNCTION();

        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            AXIS_CORE_ERROR("Could not open {0} file", filename);
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    std::string VulkanShader::Compile(const std::string& shaderSource)
    {
        AXIS_PROFILE_FUNCTION();

        // Compile glsl to spir-v
        return ""; // Name of compiled shader
    }

    void VulkanShader::Bind() const
    {
        AXIS_PROFILE_FUNCTION();

    }

    void VulkanShader::Unbind() const
    {
        AXIS_PROFILE_FUNCTION();

    }

    void VulkanShader::SetMat4(const std::string name, const glm::mat4& value)
    {
        AXIS_PROFILE_FUNCTION();

        UploadUniformMat4(name, value);
    }

    void VulkanShader::SetFloat4(const std::string name, const glm::vec4& value)
    {
        AXIS_PROFILE_FUNCTION();

        UploadUniformFloat4(name, value);
    }

    void VulkanShader::SetFloat3(const std::string name, const glm::vec3& value)
    {
        AXIS_PROFILE_FUNCTION();

        UploadUniformFloat3(name, value);
    }

    void VulkanShader::SetFloat(const std::string name, const float& value)
    {
        AXIS_PROFILE_FUNCTION();

        UploadUniformFloat(name, value);
    }

    void VulkanShader::SetInt(const std::string name, int value)
    {
        AXIS_PROFILE_FUNCTION();

        UploadUniformInt(name, value);
    }

    void VulkanShader::SetIntArray(const std::string name, int* values, int32_t count)
    {
        UploadUniformIntArray(name, values, count);
    }

    void VulkanShader::UploadUniformInt(const std::string& name, const int& value)
    {
        
    }

    void VulkanShader::UploadUniformIntArray(const std::string& name, int* values, int32_t count)
    {
        
    }

    void VulkanShader::UploadUniformFloat(const std::string& name, const float& value)
    {
        
    }

    void VulkanShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        
    }

    void VulkanShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        
    }

    void VulkanShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        
    }

    void VulkanShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        
    }

    void VulkanShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        
    }

}

