#pragma once

#include <entt.hpp>

#include <Axis/Scene/Scene.h>

namespace Axis {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AXIS_CORE_ASSERT(!HasComponent<T>(), "Entity Already Has Component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			AXIS_CORE_ASSERT(HasComponent<T>(), "Entity Does Not Have Component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			AXIS_CORE_ASSERT(HasComponent<T>(), "Entity Does Not Have Component!");
			return m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_Initialized; }
	private:
		entt::entity m_EntityHandle;
		bool m_Initialized = false;
		Scene* m_Scene = nullptr;
	};

}