#include "axispch.h"
#include "Axis/Scene/ModelImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Axis/Scene/Components.h"

namespace Axis {

	std::string s_SearchDirectory;

	ModelComponent ModelImporter::Load(const std::string& path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		AXIS_CORE_ASSERT(scene || !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode, "Failed to Load Model");

		s_SearchDirectory = path.substr(0, path.find_last_of('/') + 1);
		std::vector<MeshComponent> meshes;
		ProcessNode(scene->mRootNode, scene, meshes);

		return ModelComponent(meshes);
	}

	void ModelImporter::ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshComponent>& meshes)
	{
		// process all the node’s meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene));
			AXIS_INFO("Mesh Processed!");
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, meshes);
		}
	}

	MeshComponent ModelImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Axis::Vertex3D> vertices;
		std::vector<uint32_t> indices;
		MaterialComponent material;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Axis::Vertex3D vertex;
			// process vertex positions, normals and texture coordinates
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoord = vec;
			}
			else
				vertex.TexCoord = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* materialData = scene->mMaterials[mesh->mMaterialIndex];
			material.DiffuseMap = LoadMaterialTextures(materialData, aiTextureType_DIFFUSE, TextureType::Diffuse);
			material.SpecularMap = LoadMaterialTextures(materialData, aiTextureType_SPECULAR, TextureType::Specular);
		}

		Ref<VertexArray> vertexArray = VertexArray::Create();

		Ref<VertexBuffer> VB;
		VB = VertexBuffer::Create((float*)vertices.data(), vertices.size() * sizeof(Vertex3D));

		VB->SetLayout({
			   { ShaderDataType::Float3, "a_Position" },
			   { ShaderDataType::Float3, "a_Normal" },
			   { ShaderDataType::Float2, "a_TexCoord" }
			});
		vertexArray->AddVertexBuffer(VB);

		Ref<IndexBuffer> IB;
		IB = IndexBuffer::Create((uint32_t*)indices.data(), indices.size());
		vertexArray->SetIndexBuffer(IB);

		return MeshComponent(vertexArray, material);
	}

	Ref<Texture2D> ModelImporter::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
	{
		if (mat->GetTextureCount(type) > 0)
		{
			aiString str;
			mat->GetTexture(type, 0, &str);
			return Texture2D::Create(s_SearchDirectory + std::string(str.C_Str()));
		}
		else {
			switch (type) {
			case aiTextureType_DIFFUSE:
			{
				auto texture = Texture2D::Create(1, 1);
				uint32_t color = 0xcccccc;
				texture->SetData(&color, sizeof(color));
				return texture;
			}
			case aiTextureType_SPECULAR:
			{
				auto texture = Texture2D::Create(1, 1);
				uint32_t color = 0x000000;
				texture->SetData(&color, sizeof(color));
				return texture;
			}
			}
		}

		return nullptr;
	}
}