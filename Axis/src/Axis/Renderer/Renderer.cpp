#include "axispch.h"
#include "Renderer.h"

namespace Axis {

	Renderer::SceneData* Renderer::m_Scene = new Renderer::SceneData();

	void Renderer::BeginScene(Camera& camera)
	{
		m_Scene->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_Scene->ViewProjectionMatrix);
		RenderCommand::DrawIndexed(vertexArray);
	}

}