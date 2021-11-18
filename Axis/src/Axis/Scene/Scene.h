#pragma once

#include <entt.hpp>

#include "Axis/Core/Timestep.h"

namespace Axis {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "");

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}