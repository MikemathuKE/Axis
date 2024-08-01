#include "axispch.h"

#include "Axis/Renderable/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Axis {

	Model::Model(const char* path)
	{
		LoadModel(path);
	}

	void Model::Draw(Axis::Ref<Axis::Shader>& shader)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i]->Draw(shader);
	}

	Axis::Ref<Model> Model::Create(const char* path)
	{
		return Axis::CreateRef<Model>(path);
	}

	void Model::LoadModel(std::string path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
			!scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}

		m_Directory = path.substr(0, path.find_last_of('/') + 1);
		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process all the node’s meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
			AXIS_INFO("Mesh Processed!");
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Axis::Ref<Axis::Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Axis::Vertex3D> vertices;
		std::vector<uint32_t> indices;
		Material material;

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
			material.Diffuse = LoadMaterialTextures(materialData, aiTextureType_DIFFUSE, TextureType::Diffuse);
			material.Specular = LoadMaterialTextures(materialData, aiTextureType_SPECULAR, TextureType::Specular);
		}

		return Axis::CreateRef<Axis::Mesh>(vertices, indices, material);
	}

	Ref<Texture2D> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			return Texture2D::Create(m_Directory + std::string(str.C_Str()));
		}

		switch (type) {
		case aiTextureType_DIFFUSE:
		{
			auto texture = Texture2D::Create( 1, 1);
			uint32_t color = 0xcccccc;
			texture->SetData( &color, sizeof(color));
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
		return nullptr;
	}

}