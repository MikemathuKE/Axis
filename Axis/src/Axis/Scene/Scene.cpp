#include "axispch.h"
#include "Axis/Scene/Scene.h"

#include <glm/glm.hpp>

#include "Axis/Scene/Entity.h"
#include "Axis/Scene/Components.h"

#include "Axis/Renderer/Renderer2D.h""
#include "Axis/Renderer/Renderer.h"

namespace Axis {

	Ref<Shader> s_FlatColorShader;

	Scene::Scene()
	{
		s_FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
		s_FlatColorShader->Bind();
		s_FlatColorShader->SetFloat4("u_Color", glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.Instance) {
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
				
				nsc.Instance->OnUpdate(ts);
			});
		}

		//Render2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera) {
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
					Renderer2D::DrawQuad(transform, sprite.Color);
				}
			}
			Renderer2D::EndScene();

			Renderer::BeginScene(*mainCamera, *cameraTransform);
			{
				auto view = m_Registry.view<TransformComponent, ModelComponent>();
				for (auto entity : view)
				{
					auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);
					for (auto& mesh : model.Meshes)
						Renderer::Submit(s_FlatColorShader, mesh.VAO, transform.Transform);
				}
			}
			Renderer::EndScene();
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		//Resize on non-fixed ratio
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

}