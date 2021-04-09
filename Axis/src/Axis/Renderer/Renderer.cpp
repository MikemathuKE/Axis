#include "axispch.h"
#include "Renderer.h"

namespace Axis {

	void Renderer::BeginScene()
	{}

	void Renderer::EndScene()
	{}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray)
	{
		RenderCommand::DrawIndexed(vertexArray);
	}

}