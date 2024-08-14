#include "axispch.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

#include <vulkan/vulkan.h>

namespace Axis {

	void VulkanRendererAPI::Init()
	{
		AXIS_PROFILE_FUNCTION();

		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);*/
	}

	void VulkanRendererAPI::SetClearColor(const glm::vec4& color)
	{
		//glClearColor(color.r, color.g, color.b, color.a);
	}

	void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		//glViewport(x, y, width, height);
	}

	void VulkanRendererAPI::Clear()
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		/*uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);*/
	}

}