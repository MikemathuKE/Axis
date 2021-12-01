#pragma once

#include <glm/glm.hpp>

#include "Axis/Scene/SceneCamera.h"
#include "Axis/Scene/ScriptableEntity.h"

#include "Axis/Renderable/Model.h"
#include "Axis/Scene/ModelImporter.h"

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
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct MeshComponent
	{
		Ref<VertexArray> VAO;

		MeshComponent(const Ref<VertexArray>& vertexArray)
			: VAO(vertexArray) {}
		MeshComponent(const MeshComponent& other) = default;
	};

	struct ModelComponent
	{
		std::vector<MeshComponent> Meshes;

		ModelComponent(const std::string& path) { *this = ModelImporter::Load(path); }
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