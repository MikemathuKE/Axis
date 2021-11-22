#include "Panels/SceneHierarchyPanel.h"

#include <imgui/imgui.h>

#include <glm/glm.hpp>
#define NK_INCLUDE_STANDARD_VARARGS
#include <Axis/Nuklear/NuklearUtils.h>

#include <Axis/Scene/Components.h>
#include <Nuklear/demo/overview.c>

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

		ImGui::End();
	}

	void SceneHierarchyPanel::OnNuklearRender()
	{
		auto ctx = Nuklear::GetContext();
		if (nk_begin(ctx, "Scene_Hierarchy", { 0, 0, 250, 250 }, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE)) 
		{
			m_Context->m_Registry.each([&](auto entityID) {
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNodeNuklear(entity);
			});
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
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawEntityNodeNuklear(Entity entity)
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
	}

}