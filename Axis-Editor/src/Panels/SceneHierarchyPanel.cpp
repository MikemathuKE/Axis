#include "Panels/SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#define NK_INCLUDE_STANDARD_VARARGS
#include <Axis/Nuklear/NuklearUtils.h>

#include <Axis/Scene/Components.h>
#include <Nuklear/demo/overview.c>

#include <glm/gtc/type_ptr.hpp>

namespace Axis {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID) {
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNodeImGui(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext) 
		{
			DrawComponentsImGui(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::OnNuklearRender()
	{
		auto ctx = Nuklear::GetContext();
		if (nk_begin(ctx, "Scene_Hierarchy", { 0, 0, 250, 250 }, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE)) 
		{
			bool selection_found = false;
			if (nk_window_is_hovered(ctx) && nk_input_is_mouse_pressed(&ctx->input, NK_BUTTON_LEFT))
			{
				m_SelectionContext = {};
			}
			nk_style_push_vec2(ctx, &ctx->style.tab.padding, nk_vec2( 0, 0 ));
			m_Context->m_Registry.each([&](auto entityID) {
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNodeNuklear(entity);
			});
			nk_style_pop_vec2(ctx);
		}
		nk_end(ctx);

		if (nk_begin(ctx, "Properties", { 0, 0, 250, 250 }, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE))
		{
			if (m_SelectionContext)
				DrawComponentsNuklear(m_SelectionContext);
		}
		nk_end(ctx);

		overview(ctx);
	}

	void SceneHierarchyPanel::DrawEntityNodeImGui(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		if (opened)
		{
			ImGui::Text("%s", tag.c_str());
			ImGui::TreePop();
		}
	}

	bool SceneHierarchyPanel::DrawEntityNodeNuklear(Entity entity)
	{
		auto ctx = Nuklear::GetContext();
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		int clicked = (m_SelectionContext == entity) ? 1 : 0;
		if(nk_tree_element_push_id(ctx, NK_TREE_NODE, tag.c_str(), NK_MINIMIZED, &clicked, (uint32_t)entity))
		{
			nk_layout_row_dynamic(ctx, Nuklear::widget_height, 1);
			nk_label(ctx, tag.c_str(), NK_TEXT_LEFT);
			nk_tree_pop(ctx);
		}
		if (clicked) {
			m_SelectionContext = entity;
		}
		return clicked == 0 ? false : true;
	}

	static void DrawImGuiVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		
		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponentsImGui(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& tc = entity.GetComponent<TransformComponent>();
				DrawImGuiVec3Control("Translation", tc.Translation);
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawImGuiVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);
				DrawImGuiVec3Control("Scale", tc.Scale, 1.0f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++) 
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
							currentProjectionTypeString = projectionTypeStrings[i];
							cameraComponent.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed AspectRatio", &cameraComponent.FixedAspectRatio);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &verticalFOV))
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& src = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));

				ImGui::TreePop();
			}
		}
	}

	static void DrawNuklearVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		auto ctx = Nuklear::GetContext();
		const float maxPropertyStep = 1000.0f;

		nk_layout_row_template_begin(ctx, Nuklear::widget_height);
		nk_layout_row_template_push_static(ctx, 80);
		nk_layout_row_template_push_dynamic(ctx);
		nk_layout_row_template_push_dynamic(ctx);
		nk_layout_row_template_push_dynamic(ctx);
		nk_layout_row_template_end(ctx);

		nk_label(ctx, label.c_str(), NK_TEXT_ALIGN_LEFT);
		nk_style_push_float(ctx, &ctx->style.property.rounding, 0.0f);
		nk_style_push_vec2(ctx, &ctx->style.property.padding, nk_vec2(2, 0));

		nk_colorf normal = { 0.8f, 0.3f, 0.25f, 1.0f };
		nk_colorf hover = { 0.9f, 0.2f, 0.2f, 1.0f };
		nk_style_push_color(ctx, &ctx->style.property.label_normal, nk_rgb_cf(normal));
		nk_style_push_color(ctx, &ctx->style.property.hover.data.color, nk_rgb_cf(hover));
		nk_style_push_color(ctx, &ctx->style.property.active.data.color, nk_rgb_cf(normal));
		values.x = nk_propertyf(ctx, "#X", values.x - maxPropertyStep, values.x, values.x + maxPropertyStep, 0.5f, 0.1f);
		nk_style_pop_color(ctx);
		nk_style_pop_color(ctx);
		nk_style_pop_color(ctx);

		normal = { 0.2f, 0.7f, 0.3f, 1.0f };
		hover = { 0.3f, 0.8f, 0.3f, 1.0f };
		nk_style_push_color(ctx, &ctx->style.property.label_normal, nk_rgb_cf(normal));
		nk_style_push_color(ctx, &ctx->style.property.hover.data.color, nk_rgb_cf(hover));
		nk_style_push_color(ctx, &ctx->style.property.active.data.color, nk_rgb_cf(normal));
		values.y = nk_propertyf(ctx, "#Y", values.y - maxPropertyStep, values.y, values.y + maxPropertyStep, 0.5f, 0.1f);
		nk_style_pop_color(ctx);
		nk_style_pop_color(ctx);
		nk_style_pop_color(ctx);

		normal = { 0.1f, 0.45f, 0.8f, 1.0f };
		hover = { 0.2f, 0.35f, 0.9f, 1.0f };
		nk_style_push_color(ctx, &ctx->style.property.label_normal, nk_rgb_cf(normal));
		nk_style_push_color(ctx, &ctx->style.property.hover.data.color, nk_rgb_cf(hover));
		nk_style_push_color(ctx, &ctx->style.property.active.data.color, nk_rgb_cf(normal));
		values.z = nk_propertyf(ctx, "#Z", values.z - maxPropertyStep, values.z, values.z + maxPropertyStep, 0.5f, 0.1f);
		nk_style_pop_color(ctx);
		nk_style_pop_color(ctx);
		nk_style_pop_color(ctx);

		nk_style_pop_vec2(ctx);
		nk_style_pop_float(ctx);
	}

	void SceneHierarchyPanel::DrawComponentsNuklear(Entity entity)
	{
		auto ctx = Nuklear::GetContext();
		const float maxPropertyStep = 1000.0f;

		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			static int edit_mode;

			static char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			nk_layout_row_template_begin(ctx, Nuklear::edit_height);
			nk_layout_row_template_push_static(ctx, 30);
			nk_layout_row_template_push_dynamic(ctx);
			nk_layout_row_template_end(ctx);
			nk_label(ctx, "Tag", NK_TEXT_ALIGN_LEFT);
			edit_mode = nk_edit_string_zero_terminated(ctx, NK_EDIT_SIMPLE | NK_EDIT_SELECTABLE | NK_EDIT_GOTO_END_ON_ACTIVATE, buffer, 256, nk_filter_default);
			if (edit_mode & NK_EDIT_ACTIVE) {
				tag = std::string(buffer);
			}
			
		}
		
		if (entity.HasComponent<TransformComponent>())
		{
			if (nk_tree_push_id(ctx, NK_TREE_NODE, "Transform", NK_MAXIMIZED, typeid(TransformComponent).hash_code()))
			{
				auto& tc = entity.GetComponent<TransformComponent>();
				DrawNuklearVec3Control("Translation", tc.Translation);
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawNuklearVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);
				DrawNuklearVec3Control("Scale", tc.Scale, 1.0f);
				nk_tree_pop(ctx);
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (nk_tree_push_id(ctx, NK_TREE_NODE, "Camera", NK_MAXIMIZED, typeid(CameraComponent).hash_code()))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				nk_bool primaryCamera = cameraComponent.Primary ? nk_true : nk_false;
				nk_checkbox_label(ctx, "Primary", &primaryCamera);
				cameraComponent.Primary = primaryCamera ? true : false;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				nk_layout_row_dynamic(ctx, Nuklear::widget_height, 1);
				int selected = nk_combo(ctx, projectionTypeStrings, 2, (int)camera.GetProjectionType(), Nuklear::widget_height, nk_vec2(nk_widget_width(ctx), 55));
				if (selected != (int)camera.GetProjectionType())
					cameraComponent.Camera.SetProjectionType((SceneCamera::ProjectionType)selected);

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					float newSize = nk_propertyf(ctx, "Size", orthoSize - maxPropertyStep, orthoSize, orthoSize + maxPropertyStep, 0.5f, 0.2f);
					if (orthoSize != newSize)
						camera.SetOrthographicSize(newSize);

					float orthoNear = camera.GetOrthographicNearClip();
					float newNear = nk_propertyf(ctx, "Near", orthoNear - maxPropertyStep, orthoNear, orthoNear + maxPropertyStep, 0.5f, 0.2f);
					if (orthoNear != newNear)
						camera.SetOrthographicNearClip(newNear);

					float orthoFar = camera.GetOrthographicFarClip();
					float newFar = nk_propertyf(ctx, "Far", orthoFar - maxPropertyStep, orthoFar, orthoFar + maxPropertyStep, 0.5f, 0.2f);
					if (orthoFar != newFar)
						camera.SetOrthographicFarClip(newFar);

					nk_bool primaryCamera = cameraComponent.FixedAspectRatio ? nk_true : nk_false;
					nk_checkbox_label(ctx, "Primary", &primaryCamera);
					cameraComponent.FixedAspectRatio = primaryCamera ? true : false;
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					float newFOV = nk_propertyf(ctx, "Vertical FOV", verticalFOV - maxPropertyStep, verticalFOV, verticalFOV + maxPropertyStep, 0.5f, 0.2f);
					if (verticalFOV != newFOV)
						camera.SetPerspectiveVerticalFOV(glm::radians(newFOV));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					float newNear = nk_propertyf(ctx, "Near", perspectiveNear - maxPropertyStep, perspectiveNear, perspectiveNear + maxPropertyStep, 0.5f, 0.2f);
					if (perspectiveNear != newNear)
						camera.SetPerspectiveNearClip(newNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					float newFar = nk_propertyf(ctx, "Far", perspectiveFar - maxPropertyStep, perspectiveFar, perspectiveFar + maxPropertyStep, 0.5f, 0.2f);
					if (perspectiveFar != newFar)
						camera.SetPerspectiveFarClip(newFar);
				}

				nk_tree_pop(ctx);
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (nk_tree_push_id(ctx, NK_TREE_NODE, "Color", NK_MAXIMIZED, typeid(SpriteRendererComponent).hash_code()))
			{
				nk_colorf color;
				auto& src = entity.GetComponent<SpriteRendererComponent>();
				color << src.Color;
				nk_layout_row_dynamic(ctx, Nuklear::color_height, 1);
				if (nk_combo_begin_color(ctx, nk_rgb_cf(color), nk_vec2(nk_widget_width(ctx), 450))) {
					nk_layout_row_dynamic(ctx, 120, 1);
					color = nk_color_picker(ctx, color, NK_RGBA);
					nk_layout_row_dynamic(ctx, 20, 2);
					color.r = nk_propertyf(ctx, "#R:", 0, color.r, 1.0f, 0.01f, 0.005f);
					color.g = nk_propertyf(ctx, "#G:", 0, color.g, 1.0f, 0.01f, 0.005f);
					color.b = nk_propertyf(ctx, "#B:", 0, color.b, 1.0f, 0.01f, 0.005f);
					color.a = nk_propertyf(ctx, "#A:", 0, color.a, 1.0f, 0.01f, 0.005f);
					nk_combo_end(ctx);
				}
				src.Color << color;
				nk_tree_pop(ctx);
			}
		}
	}
}