#include "axispch.h"

#include "Axis/Nuklear/NuklearLayer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#define NK_INCLUDE_DEFAULT_SERIALIZATION

#include "Axis/Nuklear/NuklearUtils.h"
#include "nuklear_glfw_opengl_impl.h"

#include "nuklear_style.h"

#include "Axis/Core/Application.h"
static struct nk_context* s_Context;

namespace Axis {

	struct nk_context* Nuklear::GetContext()
	{
		return s_Context;
	}

	NuklearLayer::NuklearLayer()
	{
		m_DebugName = "NuklearLayer";
	}

	NuklearLayer::~NuklearLayer()
	{
		OnDetach();
	}

	struct nk_context* NuklearLayer::GetContext()
	{
		return s_Context;
	}

	void NuklearLayer::OnAttach()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//* GUI */
		s_Context = nk_glfw3_init(&s_NKStruct, window, NK_GLFW3_INSTALL_CALLBACKS);
		/* Load Fonts: if none of these are loaded a default font will be used  */
		/* Load Cursor: if you uncomment cursor loading please hide the cursor */
		{
			struct nk_font_atlas* atlas;
			
			nk_glfw3_font_stash_begin(&s_NKStruct, &atlas);
			//struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/Nuklear/extra_font/DroidSans.ttf", 14, 0);
			//struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/Nuklear/extra_font/Roboto-Regular.ttf", 14, 0);
			//struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/Nuklear/extra_font/kenvector_future_thin.ttf", 13, 0);
			//struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/Nuklear/extra_font/ProggyClean.ttf", 12, 0);
			//struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/Nuklear/extra_font/ProggyTiny.ttf", 10, &nk_font_config(15));
			//struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/Nuklear/extra_font/Cousine-Regular.ttf", 13, 0);
			nk_glfw3_font_stash_end(&s_NKStruct);
			
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			nk_style_load_all_cursors(s_Context, atlas->cursors);
			//nk_style_set_font(m_Context, &tiny->handle);

			s_Context->style.property.padding = nk_vec2(2, 0);
		}

		//set_style(s_Context, THEME_NONE);
	}

	void NuklearLayer::OnDetach()
	{
		nk_glfw3_shutdown(&s_NKStruct);
	}

	void NuklearLayer::OnEvent(Event& event)
	{
		if (m_BlockEvents)
			event.Handled = true;
		else
			event.Handled = false;
	}

	void NuklearLayer::Begin()
	{
		nk_glfw3_new_frame(&s_NKStruct);
	}

	void NuklearLayer::End()
	{
		nk_glfw3_render(&s_NKStruct, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
	}

}
