#include "axispch.h"

#include "Axis/Renderer/Renderer.h"
#include "Axis/Renderer/FrameBuffer.h"
#include "Platform/OpenGL/OpenGLFramBuffer.h"

namespace Axis {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: AXIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
		}

		AXIS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}