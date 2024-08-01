#include "axispch.h"
#include "Axis/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Axis {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}