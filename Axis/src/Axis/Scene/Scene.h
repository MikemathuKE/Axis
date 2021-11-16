#pragma once

#include <entt.hpp>

#include "Axis/Core/Timestep.h"

namespace Axis {

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		//TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
	};

}