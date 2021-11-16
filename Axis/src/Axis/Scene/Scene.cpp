#include "axispch.h"
#include "Axis/Scene/Scene.h"

#include <glm/glm.hpp>

#include <Axis/Scene/Components.h>
#include <Axis/Renderer/Renderer2D.h>

namespace Axis {

	Scene::Scene()
	{
		#if 0
		struct MeshComponent
		{
			float data;
			MeshComponent() = default;
		};

		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view) 
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto&[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}

		m_Registry.remove<TransformComponent>(entity);
		#endif
	}

	Scene::~Scene()
	{

	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

}