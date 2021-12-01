#pragma once

#include "Axis/Renderable/Mesh.h"

#include <assimp/scene.h>

namespace Axis {

	class Model
	{
	public:
		Model(const char* path);
		void Draw(Axis::Ref<Axis::Shader>& shader);
		void SetPosition(const glm::vec3& pos) {
			for (auto& mesh : m_Meshes)
				mesh->SetPosition(pos);
		}
		static Axis::Ref<Model> Create(const char* path);
	private:
		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Axis::Ref<Axis::Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Axis::TextureData> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
	private:
		std::vector<Axis::Ref<Axis::Mesh>> m_Meshes;
		std::string m_Directory;
	};

}