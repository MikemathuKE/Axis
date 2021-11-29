#include "Panels/SceneHierarchyPanel.h"

#include <imgui/imgui.h>

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
			nk_layout_row_dynamic(ctx, 20, 1);
			nk_label(ctx, tag.c_str(), NK_TEXT_LEFT);
			nk_tree_pop(ctx);
		}
		if (clicked) {
			m_SelectionContext = entity;
		}
		return clicked == 0 ? false : true;
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
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.25f);

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

			nk_layout_row_dynamic(ctx, 20, 1);
			edit_mode = nk_edit_string_zero_terminated(ctx, NK_EDIT_SIMPLE | NK_EDIT_SELECTABLE | NK_EDIT_GOTO_END_ON_ACTIVATE, buffer, 256, nk_filter_default);
			if (edit_mode & NK_EDIT_ACTIVE) {
				tag = std::string(buffer);
			}
			
		}
		
		if (entity.HasComponent<TransformComponent>())
		{
			if (nk_tree_push_id(ctx, NK_TREE_NODE, "Transform", NK_MAXIMIZED, typeid(TransformComponent).hash_code()))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				nk_layout_row_dynamic(ctx, 20, 3);
				transform[3][0] = nk_propertyf(ctx, "X", transform[3][0] - maxPropertyStep, transform[3][0], transform[3][0] + maxPropertyStep, 0.5f, 0.1f);
				transform[3][1] = nk_propertyf(ctx, "Y", transform[3][1] - maxPropertyStep, transform[3][1], transform[3][1] + maxPropertyStep, 0.5f, 0.1f);
				transform[3][2] = nk_propertyf(ctx, "Z", transform[3][2] - maxPropertyStep, transform[3][2], transform[3][2] + maxPropertyStep, 0.5f, 0.1f);
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
				nk_layout_row_dynamic(ctx, 20, 1);
				int selected = nk_combo(ctx, projectionTypeStrings, 2, (int)camera.GetProjectionType(), 20, nk_vec2(nk_widget_width(ctx), 55));
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
	}
}