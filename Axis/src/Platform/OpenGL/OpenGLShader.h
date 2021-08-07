#pragma once

#include "Axis/Renderer/Shader.h"

namespace Axis {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override { UploadUniformMat4(name, matrix); }
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix)  override { UploadUniformMat3(name, matrix); };

		virtual void SetFloat4(const std::string& name, const glm::vec4& values) override { UploadUniformFloat4(name, values); }
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) override { UploadUniformFloat3(name, values); }
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) override { UploadUniformFloat2(name, values); }
		virtual void SetFloat(const std::string& name, const float value) override { UploadUniformFloat(name, value); }

		virtual void SetInt(const std::string& name, const int value) override { UploadUniformInt(name, value); }

	private:
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);

		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat(const std::string& name, const float value);

		void UploadUniformInt(const std::string& name, const int value);
	private:
		uint32_t m_RendererID;
	};

}