#pragma once

#include "Axis.h"

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
		OrthographicCameraController m_CameraController;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		//Temp
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteSheet;

		Ref<FrameBuffer> m_FrameBuffer;

		Ref<SubTexture2D> m_TextureStairs;
		Ref<SubTexture2D> m_TextureBarrel;
		Ref<SubTexture2D> m_TextureTree;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
		uint32_t m_MapWidth, m_MapHeight;
	};

}