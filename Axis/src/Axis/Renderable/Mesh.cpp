#include "axispch.h"

#include "Axis/Renderable/Mesh.h"
#include "Axis/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Axis {

	static const char* TextureTypeToString(TextureType type)
	{
		switch (type) {
		case TextureType::Diffuse: return "material.diffuse";
		case TextureType::Specular: return "material.specular";
		}

		AXIS_CORE_ASSERT(false, "Texture Type Not Supported!");
		return "";
	}

	Mesh::Mesh(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices,
		const std::vector<TextureData>& textureData)
		:m_Vertices(vertices), m_Indices(indices), m_TextureData(textureData)
	{
		SetupMesh();
	}

	void Mesh::Draw(Ref<Shader>& shader)
	{
		shader->Bind();
		for (unsigned int i = 0; i < m_TextureData.size(); i++)
		{
			m_TextureData[i].Texture->Bind(i);
			shader->SetInt(TextureTypeToString(m_TextureData[i].Type), i);
		}
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_Position);
		Renderer::Submit(shader, m_VertexArray, translate);
	}

	Ref<Mesh> Mesh::Create(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices,
		const std::vector<TextureData>& textureData)
	{
		return CreateRef<Mesh>(vertices, indices, textureData);
	}

	void Mesh::SetupMesh() {
		m_VertexArray = VertexArray::Create();

		Ref<VertexBuffer> VB;
		VB = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(Vertex3D));

		VB->SetLayout({
			   { ShaderDataType::Float3, "a_Position" },
			   { ShaderDataType::Float3, "a_Normal" },
			   { ShaderDataType::Float2, "a_TexCoord" }
			});
		m_VertexArray->AddVertexBuffer(VB);

		Ref<IndexBuffer> IB;
		IB = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_Indices.size());
		m_VertexArray->SetIndexBuffer(IB);
	}

}