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

	//Temp
	Axis::Ref<Axis::Texture2D> m_Texture;

	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
};