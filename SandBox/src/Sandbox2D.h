#pragma once

#include "Axis.h"

class Sandbox2D : public Axis::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Axis::Timestep ts) override;
	virtual void OnGUIRender() override;
	virtual void OnEvent(Axis::Event& e) override;
private:
	Axis::OrthographicCameraController m_CameraController;

	// Temp
	Axis::Ref<Axis::Shader> m_FlatColorShader;
	Axis::Ref<Axis::VertexArray> m_SquareVA;

	glm::vec4 m_SquareColor = { 0.9f, 0.8f, 0.1f, 1.0f };
};