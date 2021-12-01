#pragma once

#include <assimp/scene.h>

namespace Axis {

	struct ModelComponent;
	struct MeshComponent;

	class ModelImporter
	{
	public:
		static ModelComponent Load(const std::string& path);
	private:
		static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshComponent>& meshes);
		static MeshComponent ProcessMesh(aiMesh* mesh, const aiScene* scene);
		//static std::vector<Axis::TextureData> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
	};

}