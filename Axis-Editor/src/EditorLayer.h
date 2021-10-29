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

		virtual void OnUpdate(Axis::Timestep ts) override;
		virtual void OnGUIRender() override;
		virtual void OnEvent(Axis::Event& e) override;
	private:
		OrthographicCameraController m_CameraController;

		//Temp
		Ref<Axis::Texture2D> m_Texture;
		Ref<Axis::Texture2D> m_SpriteSheet;

		Ref<Axis::FrameBuffer> m_FrameBuffer;

		Ref<Axis::SubTexture2D> m_TextureStairs;
		Ref<Axis::SubTexture2D> m_TextureBarrel;
		Ref<Axis::SubTexture2D> m_TextureTree;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
		std::unordered_map<char, Axis::Ref<SubTexture2D>> s_TextureMap;
		uint32_t m_MapWidth, m_MapHeight;
	};

}