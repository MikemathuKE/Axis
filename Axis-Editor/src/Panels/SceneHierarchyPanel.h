#pragma once

#include <Axis/Core/Base.h>
#include <Axis/Core/Log.h>
#include <Axis/Scene/Scene.h>
#include <Axis/Scene/Entity.h>

namespace Axis {
	
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
		void OnNuklearRender();
	private:
		void DrawEntityNodeImGui(Entity entity);
		bool DrawEntityNodeNuklear(Entity entity);

		void DrawComponentsImGui(Entity entity);
		void DrawComponentsNuklear(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}
