#include "axispch.h"

#include "NuklearLayer.h"

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

#include <Nuklear/nuklear.h>
#include "nuklear_glfw_opengl_impl.h"

#include <Nuklear/demo/overview.c>
#include "nuklear_style.h"

#include "Axis/Core/Application.h"

static struct nk_glfw s_NK_GLFW;

namespace Axis {

	NuklearLayer::NuklearLayer()
		:Layer("NuklearLayer")
	{
	}

	NuklearLayer::~NuklearLayer()
	{
		OnDetach();
	}

	void NuklearLayer::OnAttach()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//* GUI */
		m_Context = nk_glfw3_init(&s_NK_GLFW, window, NK_GLFW3_DEFAULT);
		/* Load Fonts: if none of these are loaded a default font will be used  */
		/* Load Cursor: if you uncomment cursor loading please hide the cursor */
		{
			struct nk_font_atlas* atlas;
			nk_glfw3_font_stash_begin(&s_NK_GLFW, &atlas);
			//struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/nuklear/extra_font/DroidSans.ttf", 14, 0);
			//struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/nuklear/extra_font/Roboto-Regular.ttf", 14, 0);
			//struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/nuklear/extra_font/kenvector_future_thin.ttf", 13, 0);
			//struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/nuklear/extra_font/ProggyClean.ttf", 12, 0);
			//struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/nuklear/extra_font/ProggyTiny.ttf", 10, 0);
			//struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../Axis/vendor/nuklear/extra_font/Cousine-Regular.ttf", 13, 0);
			nk_glfw3_font_stash_end(&s_NK_GLFW);
			/*nk_style_load_all_cursors(ctx, atlas->cursors);*/
			/*nk_style_set_font(ctx, &droid->handle);*/
		}

		//set_style(m_Context, THEME_WHITE);
		//set_style(m_Context, THEME_RED);
		//set_style(m_Context, THEME_BLUE);
		set_style(m_Context, THEME_AXIS);
	}

	void NuklearLayer::OnDetach()
	{
		nk_glfw3_shutdown(&s_NK_GLFW);
	}

	void NuklearLayer::Begin()
	{
		nk_glfw3_new_frame(&s_NK_GLFW);
	}

	void NuklearLayer::OnGUIRender()
	{
		overview(m_Context);
	}

	void NuklearLayer::End()
	{
		nk_glfw3_render(&s_NK_GLFW, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
	}

}
