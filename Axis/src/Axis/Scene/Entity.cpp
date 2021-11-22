#include "axispch.h"
#include "Axis/Scene/Entity.h"

namespace Axis {

	Entity::Entity(entt::entity handle, Scene* scene)
		:  m_EntityHandle(handle), m_Scene(scene)
	{}

}