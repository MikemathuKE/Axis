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
static struct nk_font_atlas* s_FontAtlas;

namespace Axis {

	struct nk_context* Nuklear::GetContext()
	{
		return s_Context;
	}

	struct nk_user_font* Nuklear::GetFont(uint32_t index)
	{
		return &s_FontAtlas->fonts[index].handle;
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
			struct nk_font *opensansBold = nk_font_atlas_add_from_file(atlas, "assets/fonts/OpenSans/OpenSans-Bold.ttf", 17, 0);
			struct nk_font *opensans = nk_font_atlas_add_from_file(atlas, "assets/fonts/OpenSans/OpenSans-Regular.ttf", 17, 0);
			nk_glfw3_font_stash_end(&s_NKStruct);
			
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			nk_style_load_all_cursors(s_Context, atlas->cursors);
			nk_style_set_font(s_Context, &opensans->handle);
		}

		s_FontAtlas = &s_NKStruct.atlas;
		SetDarkThemeColors();

		s_Context->style.property.padding = nk_vec2(2, 0);
		s_Context->style.window.min_size.x = 370;
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

	void NuklearLayer::SetDarkThemeColors()
	{
		struct nk_color table[NK_COLOR_COUNT];
		table[NK_COLOR_TEXT] = nk_rgba(255, 255, 255, 255);

		table[NK_COLOR_WINDOW] = nk_rgba(29, 30, 31, 255);
		table[NK_COLOR_HEADER] = nk_rgba(42, 43, 44, 255);
		table[NK_COLOR_BORDER] = nk_rgba(56, 56, 56, 255);

		table[NK_COLOR_BUTTON] = nk_rgba(50, 58, 61, 225);
		table[NK_COLOR_BUTTON_HOVER] = nk_rgba(60, 68, 71, 225);
		table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(70, 78, 71, 225);

		table[NK_COLOR_TOGGLE] = nk_rgba(50, 58, 61, 255);
		table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 90, 100, 255);
		table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 103, 181, 255);

		table[NK_COLOR_SELECT] = table[NK_COLOR_WINDOW];
		table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 50, 52, 255);

		table[NK_COLOR_SLIDER] = nk_rgba(50, 58, 61, 255);
		table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(48, 103, 181, 255);
		table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(52, 123, 201, 255);
		table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = table[NK_COLOR_SLIDER_CURSOR_HOVER];

		table[NK_COLOR_PROPERTY] = nk_rgba(50, 58, 61, 255);

		table[NK_COLOR_EDIT] = nk_rgba(50, 58, 61, 225);
		table[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);

		table[NK_COLOR_COMBO] = nk_rgba(50, 58, 61, 255);

		table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
		table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 103, 181, 255);
		table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);

		table[NK_COLOR_SCROLLBAR] = nk_rgba(50, 58, 61, 255);
		table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(108, 113, 116, 255);
		table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(128, 133, 136, 255);
		table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = table[NK_COLOR_SCROLLBAR_CURSOR_HOVER];

		table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 50, 52, 255);
		nk_style_from_table(s_Context, table);
	}

}
