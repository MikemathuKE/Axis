#pragma once

#include <assimp/scene.h>
#include "Axis/Renderer/Texture.h"

namespace Axis {

	struct ModelComponent;
	struct MeshComponent;

	class Importer
	{
	public:
		static ModelComponent LoadModel(const std::string& path);
	private:
		// Model Component
		static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshComponent>& meshes);
		static MeshComponent ProcessMesh(aiMesh* mesh, const aiScene* scene);
		static Ref<Texture2D> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
	};

}