#pragma once

#include "Axis/Scene/Entity.h"
#include "Axis/Core/Timestep.h"

namespace Axis {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep) {}
	private:
		Entity m_Entity;
		friend class Scene;
	};

}