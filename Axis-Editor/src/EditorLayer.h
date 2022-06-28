#pragma once

#include <Axis.h>
#include "Panels/SceneHierarchyPanel.h"

namespace Axis {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnGUIRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		void OnImGuiRender();
		void OnNuklearRender();

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

		bool OnKeyPressed(KeyPressedEvent& e);
	private:
		Ref<Scene> m_ActiveScene;

		Ref<FrameBuffer> m_FrameBuffer;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		int m_GizmoType = -1;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}