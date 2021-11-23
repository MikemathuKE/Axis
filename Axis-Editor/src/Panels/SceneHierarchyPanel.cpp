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
	}

	void SceneHierarchyPanel::DrawComponentsNuklear(Entity entity)
	{
		auto ctx = Nuklear::GetContext();
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
				transform[3][0] = nk_propertyf(ctx, "X", transform[3][0] - 3.0f, transform[3][0], transform[3][0] + 3.0f, 0.5f, 0.1f);
				transform[3][1] = nk_propertyf(ctx, "Y", transform[3][1] - 3.0f, transform[3][1], transform[3][1] + 3.0f, 0.5f, 0.1f);
				transform[3][2] = nk_propertyf(ctx, "Z", transform[3][2] - 3.0f, transform[3][2], transform[3][2] + 3.0f, 0.5f, 0.1f);
				nk_tree_pop(ctx);
			}
		}
	}
}