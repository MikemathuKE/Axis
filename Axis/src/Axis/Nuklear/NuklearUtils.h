#pragma once

// Include glm.hpp and nuklear.h before this header
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_STANDARD_VARARGS
#include <Nuklear/nuklear.h>
#include <glm/glm.hpp>

inline glm::vec4& operator<<(glm::vec4& vec, const nk_colorf& color)
{
	vec.r = color.r;
	vec.g = color.g;
	vec.b = color.b;
	vec.a = color.a;
	return vec;
}

inline nk_colorf& operator<<(nk_colorf& color, const glm::vec4& vec)
{
	color.r = vec.r;
	color.g = vec.g;
	color.b = vec.b;
	color.a = vec.a;
	return color;
}

namespace Axis {

	enum FontType : uint32_t {
		Bold = 0, Regular
	};

	class Nuklear
	{
	public:
		static struct nk_context* GetContext();
		static struct nk_user_font* GetFont(uint32_t index);
		static float GetTextWidth(const char* text) { int32_t text_len = (int32_t)strlen(text); return GetContext()->style.font->width(GetContext()->style.font->userdata, GetContext()->style.font->height, text, text_len); }
		static float GetRowHeight() {
			return GetContext()->style.font->height + 2.0f;
		}

		//Window
		static nk_bool Begin(const char* title, struct nk_rect bounds, nk_flags flags) { return nk_begin(GetContext(), title, bounds, flags); }
		static void End() { nk_end(GetContext()); }
		static nk_bool IsWindowHovered() { return nk_window_is_hovered(GetContext()); }
		static struct nk_rect GetContentRegion() { return nk_window_get_content_region(GetContext()); }
		static struct nk_rect GetBounds() { return nk_window_get_bounds(GetContext()); }

		//group
		static nk_bool GroupBegin(const char* title, nk_flags flags) { return nk_group_begin(GetContext(), title, flags); }
		static void GroupEnd() { nk_group_end(GetContext()); }

		//Contextual
		static nk_bool ContextualBegin(nk_flags flags, struct nk_vec2 size, struct nk_rect triggerBounds, nk_buttons button = NK_BUTTON_RIGHT) { return nk_contextual_begin(GetContext(), flags, size, triggerBounds, button); }
		static void ContextualEnd() { nk_contextual_end(GetContext()); }
		static nk_bool ContextualLabel(const char* title, nk_flags align = NK_TEXT_CENTERED) { return nk_contextual_item_label(GetContext(), title, align); }
		static void ContextualClose() { nk_contextual_close(GetContext()); }

		//Color
		static void ColorEdit(glm::vec4& src_Color)
		{
			struct nk_context* ctx = GetContext();
			nk_colorf color;
			color << src_Color;

			if (nk_combo_begin_color(ctx, nk_rgb_cf(color), nk_vec2(nk_widget_width(ctx), 450))) {
				nk_layout_row_dynamic(ctx, 120, 1);
				color = nk_color_picker(ctx, color, NK_RGBA);
				SetDynamicLayout(color_height, 4);
				nk_style_push_font(ctx, GetFont(FontType::Bold));
				color.r = nk_propertyf(ctx, "#R:", 0, color.r, 1.0f, 0.01f, 0.005f);
				color.g = nk_propertyf(ctx, "#G:", 0, color.g, 1.0f, 0.01f, 0.005f);
				color.b = nk_propertyf(ctx, "#B:", 0, color.b, 1.0f, 0.01f, 0.005f);
				color.a = nk_propertyf(ctx, "#A:", 0, color.a, 1.0f, 0.01f, 0.005f);
				nk_style_pop_font(ctx);
				nk_combo_end(ctx);
			}
			src_Color << color;
		}

		//Layout
		static void SetDynamicLayout(float height = widget_height, int32_t cols = 1) { return nk_layout_row_dynamic(GetContext(), height, cols); }
		static void SetStaticLayout(int32_t width, int32_t cols = 1, float height = widget_height) { return nk_layout_row_static(GetContext(), height, width, cols); }
		static void LayoutTemplateBegin(float row_height = widget_height) { nk_layout_row_template_begin(GetContext(), row_height); }
		static void LayoutTemplatePushStatic(float width) { nk_layout_row_template_push_static(GetContext(), width); }
		static void LayoutTemplatePushDynamic() { nk_layout_row_template_push_dynamic(GetContext()); }
		static void LayoutTemplateEnd() { nk_layout_row_template_end(GetContext()); }

		//Input
		static nk_bool IsMousePressed(nk_buttons button) { return nk_input_is_mouse_pressed(&GetContext()->input, button); }
		static nk_bool IsMouseClickedDownInRect(nk_buttons button, struct nk_rect b, nk_bool down) { return nk_input_is_mouse_click_down_in_rect(&GetContext()->input, button, b, down); }

		//Style
		static void PushVec2(struct nk_vec2* address, struct nk_vec2 value) { nk_style_push_vec2(GetContext(), address, value); }
		static void PopVec2() { nk_style_pop_vec2(GetContext()); }
		static void PushFloat(float* address, float value) { nk_style_push_float(GetContext(), address, value); }
		static void PopFloat() { nk_style_pop_float(GetContext()); }
		static void PushColor(struct nk_color* address, glm::vec4 c) { nk_colorf color = { c.r, c.g, c.b, c.a }; nk_style_push_color(GetContext(), address, nk_rgb_cf(color)); }
		static void PopColor() { nk_style_pop_color(GetContext()); }
		static void PushFont(FontType type) {
			nk_style_push_font(GetContext(), GetFont(type));
		}
		static void PopFont() { nk_style_pop_font(GetContext()); }

		//Widget
		static float GetWidgetWidth() { return nk_widget_width(GetContext()); }
		static struct nk_rect GetWidgetBounds() { return nk_layout_widget_bounds(GetContext()); }

		//Property
		static float PropertyFloat(const char* label, float value, float step = 0.5f, float inc_per_pixel = 0.2f, float bound = 100000.0f) { return nk_propertyf( GetContext(), label, value - bound, value, value + bound, step, inc_per_pixel); }
		static float PropertyFloat(const char* label, float value, float min, float max, float step, float inc_per_pixel) { return nk_propertyf(GetContext(), label, min, value, max, step, inc_per_pixel); }

		//Button
		static nk_bool ButtonLabel(const char* label) { return nk_button_label(GetContext(), label); }

		//Label
		static void Label(const char* text, nk_flags align) { nk_label(GetContext(), text, align); }
		template<typename ... Args>
		static void Labelf(nk_flags align, const char* format, Args&& ... args) { nk_labelf(GetContext(), align, format, std::forward<Args>(args)...); }

		//Edit
		static int32_t EditStringZeroTerminated(nk_flags flags, char* buffer, nk_plugin_filter filter, int32_t max = 256) { return nk_edit_string_zero_terminated( GetContext(), flags, buffer, max, filter); }

		//Checkbox
		static void CheckboxLabel(const char* label, nk_bool* checked) { nk_checkbox_label(GetContext(), label, checked); }

		//Combo
		static int32_t Combo(const char** items, int count, int selected, struct nk_vec2 size, int item_height = widget_height) { return nk_combo(GetContext(), items, count, selected, item_height, size); }

		//Menu
		static nk_bool MenuLabelBegin(const char* title, nk_flags align, struct nk_vec2 size) { return nk_menu_begin_label(GetContext(), title, align, size); }
		static nk_bool MenuItemLabel(const char* title, nk_flags align = NK_TEXT_LEFT) { return nk_menu_item_label(GetContext(), title, align); }
		static void MenuEnd() { nk_menu_end(GetContext()); }

		//Image
		static void Image(struct nk_image image, nk_bool invert = nk_false) { nk_image(GetContext(), image, invert); }

		//Tree
		static nk_bool TreePush(nk_tree_type type, const char* title, nk_collapse_states state) { return nk_tree_push(GetContext(), type, title, state); }
		static void TreePop() { nk_tree_pop(GetContext()); }
		static nk_bool TreeElementPushId(nk_tree_type type, const char* title, nk_collapse_states state, nk_bool* selected, uint32_t id) { return nk_tree_element_push_id(GetContext(), type, title, state, selected, id); }
		static nk_bool TreePushId(nk_tree_type type, const char* title, nk_collapse_states state, uint32_t id) { return nk_tree_push_id(GetContext(), type, title, state, id); }


		static constexpr float widget_height = 18;
		static constexpr float edit_height = 24;
		static constexpr float color_height = 19;
	};
}
