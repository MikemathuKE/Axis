#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Axis/Scene/SceneCamera.h"
#include "Axis/Scene/ScriptableEntity.h"

#include "Axis/Renderable/Model.h"
#include "Axis/Scene/Importer.h"

#include "Axis/Core/Timestep.h"

namespace Axis {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		operator std::string& () { return Tag; }
		operator const std::string& () { return Tag; }
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation) *
				rotation *
				glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::vec3 GetTranslation() const
		{
			return Translation;
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct MaterialComponent
	{
		Ref<Texture2D> DiffuseMap;
		Ref<Texture2D> SpecularMap;
		float Shininess = 32.0f;

		MaterialComponent()
			: Shininess(32.0f)
		{
			auto DiffuseMap = Texture2D::Create(1, 1);
			uint32_t color = 0xcccccc;
			DiffuseMap->SetData(&color, sizeof(color));

			auto SpecularMap = Texture2D::Create(1, 1);
			color = 0xcccccc;
			SpecularMap->SetData(&color, sizeof(color));
		}
		MaterialComponent(const Ref<Texture2D>& diffuse, const Ref<Texture2D>& specular, float shine = 32.0f)
			: DiffuseMap(diffuse), SpecularMap(specular), Shininess(shine) {}
		MaterialComponent(const MaterialComponent& other) = default;
	};

	struct MeshComponent
	{
		Ref<VertexArray> VAO;
		Ref<MaterialComponent> Material;

		MeshComponent(const Ref<VertexArray>& vertexArray, const Ref<MaterialComponent>& material)
			: VAO(vertexArray), Material(material) {}
		MeshComponent(const MeshComponent& other) = default;
	};

	struct ModelComponent
	{
		std::vector<MeshComponent> Meshes;

		ModelComponent(const std::string& path) { *this = Importer::LoadModel(path); }
		ModelComponent(const MeshComponent& mesh) { Meshes.push_back(mesh); }
		ModelComponent(const std::vector<MeshComponent>& meshes)
			: Meshes(meshes) {}
		ModelComponent(const ModelComponent& other) = default;

		void AddMeshComponent(const MeshComponent& mesh) { Meshes.push_back(mesh); }
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = false; // Todo : Think of moving to scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}

	};

}