#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Axis/Renderer/Texture.h"
#include "Axis/Renderer/VertexArray.h"
#include "Axis/Renderer/Shader.h"

namespace Axis {

	struct Vertex3D {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
	};

	struct TextureData {
		Ref<Texture2D> Texture;
		TextureType Type;
	};

	struct Material {
		Ref<Texture2D> Diffuse;
		Ref<Texture2D> Specular;
		float Shininess = 32.0f;
	};

	class Mesh {
	public:
		Mesh(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices,
			const Material& material);
		void Draw(Ref<Shader>& shader);
		void SetPosition(const glm::vec3& pos) { m_Position = pos; }

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

		static Ref<Mesh> Create(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices,
			const Material& material);
	private:
		void SetupMesh();
	private:
		std::vector<Vertex3D> m_Vertices;
		std::vector<uint32_t> m_Indices;
		Material m_Material;

		glm::vec3 m_Position = { 0, 0, 0 };

		Ref<VertexArray> m_VertexArray;
	};

}