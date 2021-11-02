#pragma once

#include "Axis.h"

class Sandbox3D : public Axis::Layer
{
public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Axis::Timestep ts) override;
	virtual void OnGUIRender() override;
	virtual void OnEvent(Axis::Event& e) override;
private:
	Axis::PerspectiveCameraController m_CameraController;

	Axis::Ref<Axis::Shader> m_LightShader;
	Axis::Ref<Axis::Shader> m_FlatColorShader;

	Axis::Ref<Axis::Mesh> m_Mesh;
	Axis::Ref<Axis::Model> m_Model;

	glm::vec3 m_LightPosition = { 2.0f, 1.0f, 3.0f };
	glm::vec3 m_LightColor = { 1.0f, 1.0f, 1.0f };
};