#include "axispch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Axis {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}