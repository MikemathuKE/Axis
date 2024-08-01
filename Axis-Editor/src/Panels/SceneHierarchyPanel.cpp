#include "Panels/SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Axis/Nuklear/NuklearUtils.h>

#include <Axis/Scene/Components.h>
#include <Nuklear/demo/overview.c>

#include <glm/gtc/type_ptr.hpp>
#include <Axis/Core/Input.h>

#include <string.h>

namespace Axis {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
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

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity("Empty Entity");
			}

			ImGui::EndPopup();
		}

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
		nk_flags flags = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE;
		auto ctx = Nuklear::GetContext();

		if (Nuklear::Begin("Scene Hierarchy", { 0, 0, 250, 250 }, flags))
		{
			bool selection_found = false;
			if (Nuklear::IsMouseClickedDownInRect(NK_BUTTON_LEFT, Nuklear::GetContentRegion(), true))
			{
				m_SelectionContext = {};
			}
			Nuklear::PushVec2(&ctx->style.tab.padding, nk_vec2( 0, 0 ));
			m_Context->m_Registry.each([&](auto entityID) {
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNodeNuklear(entity);
			});
			Nuklear::PopVec2();
			if (Nuklear::ContextualBegin(0, nk_vec2(150, 200), Nuklear::GetContentRegion()))
			{
				Nuklear::SetDynamicLayout();
				if (Nuklear::ContextualLabel("Add Entity", NK_TEXT_CENTERED))
				{
					m_Context->CreateEntity("Empty Entity");
					Nuklear::ContextualClose();
				}
				Nuklear::ContextualEnd();
			}
		}
		Nuklear::End();

		if (Nuklear::Begin("Properties", { 0, 0, 250, 250 }, flags))
		{
			if (m_SelectionContext) {
				DrawComponentsNuklear(m_SelectionContext);
			}
		}
		Nuklear::End();

		overview(ctx);
	}

	void SceneHierarchyPanel::DrawEntityNodeImGui(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entity_deleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entity_deleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::Text("%s", tag.c_str());
			ImGui::TreePop();
		}

		if (entity_deleted) {
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
			m_Context->DestroyEntity(entity);
		}
	}

	void SceneHierarchyPanel::DrawEntityNodeNuklear(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		int selected = (m_SelectionContext == entity) ? 1 : 0;
		bool entity_deleted = false;

		nk_bool opened = Nuklear::TreeElementPushId(NK_TREE_NODE, tag.c_str(), NK_MINIMIZED, &selected, (uint32_t)entity);
		struct nk_rect bounds = Nuklear::GetWidgetBounds();
		if (Nuklear::ContextualBegin(0, nk_vec2(150, 200), bounds))
		{
			Nuklear::SetDynamicLayout();
			if (Nuklear::ContextualLabel("Delete Entity", NK_TEXT_CENTERED))
			{
				entity_deleted = true;
				Nuklear::ContextualClose();
			}
			Nuklear::ContextualEnd();
		}
		if (opened)
		{
			Nuklear::SetDynamicLayout();
			Nuklear::Label(tag.c_str(), NK_TEXT_LEFT);
			Nuklear::TreePop();
		}
		if (selected) {
			m_SelectionContext = entity;
		}
		if (entity_deleted) {
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
			m_Context->DestroyEntity(entity);
		}
	}

	static void DrawImGuiVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.25f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.25f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.45f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.45f, 0.8f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponentImGui(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
			{
				ImGui::OpenPopup("Component Settings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("Component Settings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);

				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponentsImGui(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add Component");

		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponentImGui<CameraComponent>("Camera", entity, [](auto& component) {
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
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

				ImGui::Checkbox("Fixed AspectRatio", &component.FixedAspectRatio);
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
		});

		DrawComponentImGui<TransformComponent>("Transform", entity, [](auto& component) {
			DrawImGuiVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawImGuiVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawImGuiVec3Control("Scale", component.Scale, 1.0f);
		});

		DrawComponentImGui<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});
	}

	static void DrawNuklearVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		auto ctx = Nuklear::GetContext();
		const float maxPropertyStep = 1000.0f;

		auto boldFont = Nuklear::GetFont(0);

		Nuklear::LayoutTemplateBegin();
		Nuklear::LayoutTemplatePushStatic(80);
		Nuklear::LayoutTemplatePushDynamic();
		Nuklear::LayoutTemplatePushDynamic();
		Nuklear::LayoutTemplatePushDynamic();
		Nuklear::LayoutTemplateEnd();

		Nuklear::Label(label.c_str(), NK_TEXT_ALIGN_LEFT);
		Nuklear::PushFloat(&ctx->style.property.rounding, 0.0f);
		Nuklear::PushFont(FontType::Bold);

		glm::vec4 normal = { 0.8f, 0.3f, 0.25f, 1.0f };
		glm::vec4 hover = { 0.9f, 0.2f, 0.2f, 1.0f };
		Nuklear::PushColor(&ctx->style.property.label_normal, normal);
		Nuklear::PushColor(&ctx->style.property.hover.data.color, hover);
		Nuklear::PushColor(&ctx->style.property.active.data.color, normal);
		values.x = Nuklear::PropertyFloat("#X", values.x);
		Nuklear::PopColor();
		Nuklear::PopColor();
		Nuklear::PopColor();

		normal = { 0.2f, 0.7f, 0.3f, 1.0f };
		hover = { 0.3f, 0.8f, 0.3f, 1.0f };
		Nuklear::PushColor(&ctx->style.property.label_normal, normal);
		Nuklear::PushColor(&ctx->style.property.hover.data.color, hover);
		Nuklear::PushColor(&ctx->style.property.active.data.color, normal);
		values.y = Nuklear::PropertyFloat("#Y", values.y);
		Nuklear::PopColor();
		Nuklear::PopColor();
		Nuklear::PopColor();

		normal = { 0.1f, 0.45f, 0.8f, 1.0f };
		hover = { 0.2f, 0.35f, 0.9f, 1.0f };
		Nuklear::PushColor(&ctx->style.property.label_normal, normal);
		Nuklear::PushColor(&ctx->style.property.hover.data.color, hover);
		Nuklear::PushColor(&ctx->style.property.active.data.color, normal);
		values.z = Nuklear::PropertyFloat("#Z", values.z);
		Nuklear::PopColor();
		Nuklear::PopColor();
		Nuklear::PopColor();

		Nuklear::PopFont();
		Nuklear::PopFloat();
	}

	template<typename T, typename UIFunction>
	static void DrawComponentNuklear(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();

			nk_bool open = Nuklear::TreePushId(NK_TREE_NODE, name.c_str(), NK_MAXIMIZED, (int)typeid(T).hash_code());
			bool removeComponent = false;

			struct nk_rect bounds = Nuklear::GetWidgetBounds();
			if (Nuklear::ContextualBegin(0, nk_vec2(150, 200), bounds))
			{
				Nuklear::SetDynamicLayout();
				if (Nuklear::ContextualLabel("Delete Component", NK_TEXT_CENTERED))
				{
					removeComponent = true;
					Nuklear::ContextualClose();
				}
				Nuklear::ContextualEnd();
			}

			if (open)
			{
				uiFunction(component);
				Nuklear::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponentsNuklear(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			static int edit_mode;

			static char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());

			Nuklear::LayoutTemplateBegin(Nuklear::edit_height);
			Nuklear::LayoutTemplatePushStatic(Nuklear::GetTextWidth("Tag"));
			Nuklear::LayoutTemplatePushDynamic();
			Nuklear::LayoutTemplatePushStatic(120);
			Nuklear::LayoutTemplateEnd();
			Nuklear::Label("Tag", NK_TEXT_ALIGN_LEFT);
			edit_mode = Nuklear::EditStringZeroTerminated( NK_EDIT_SIMPLE | NK_EDIT_SELECTABLE | NK_EDIT_GOTO_END_ON_ACTIVATE, buffer, nk_filter_default);
			if (edit_mode & NK_EDIT_ACTIVE) {
				tag = std::string(buffer);
			}
		}

		Nuklear::ButtonLabel("Add Component");
		struct nk_rect bounds = Nuklear::GetWidgetBounds();
		bounds.x += bounds.w - 120;
		bounds.w = 120;

		if (Nuklear::ContextualBegin(0, nk_vec2(150, 200), bounds, NK_BUTTON_LEFT))
		{
			Nuklear::SetDynamicLayout();
			if (Nuklear::ContextualLabel("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
			}
			if (Nuklear::ContextualLabel("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
			}
			Nuklear::ContextualEnd();
		}

		DrawComponentNuklear<TransformComponent>("Transform", entity, [](auto& component) {
			Nuklear::SetDynamicLayout(Nuklear::widget_height * 4, 1);
			if (Nuklear::GroupBegin("Transforms", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
			{
				DrawNuklearVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawNuklearVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawNuklearVec3Control("Scale", component.Scale, 1.0f);
			}
			Nuklear::GroupEnd();
		});

		DrawComponentNuklear<CameraComponent>("Camera", entity, [](auto& component) {
			auto& camera = component.Camera;

			nk_bool primaryCamera = component.Primary ? nk_true : nk_false;
			Nuklear::CheckboxLabel("Primary", &primaryCamera);
			component.Primary = primaryCamera ? true : false;

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			Nuklear::SetDynamicLayout();
			int selected = Nuklear::Combo(projectionTypeStrings, 2, (int)camera.GetProjectionType(), nk_vec2(Nuklear::GetWidgetWidth(), 55));
			if (selected != (int)camera.GetProjectionType())
				component.Camera.SetProjectionType((SceneCamera::ProjectionType)selected);

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				float newSize = Nuklear::PropertyFloat("Size", orthoSize);
				if (orthoSize != newSize)
					camera.SetOrthographicSize(newSize);

				float orthoNear = camera.GetOrthographicNearClip();
				float newNear = Nuklear::PropertyFloat("Near", orthoNear);
				if (orthoNear != newNear)
					camera.SetOrthographicNearClip(newNear);

				float orthoFar = camera.GetOrthographicFarClip();
				float newFar = Nuklear::PropertyFloat("Far", orthoFar);
				if (orthoFar != newFar)
					camera.SetOrthographicFarClip(newFar);

				nk_bool primaryCamera = component.FixedAspectRatio ? nk_true : nk_false;
				Nuklear::CheckboxLabel("Fixed Aspect Ratio", &primaryCamera);
				component.FixedAspectRatio = primaryCamera ? true : false;
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
				float newFOV = Nuklear::PropertyFloat("Vertical FOV", verticalFOV, 45, 75, 0.5f, 0.2f);
				if (verticalFOV != newFOV)
					camera.SetPerspectiveVerticalFOV(glm::radians(newFOV));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				float newNear = Nuklear::PropertyFloat("Near", perspectiveNear);
				if (perspectiveNear != newNear)
					camera.SetPerspectiveNearClip(newNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				float newFar = Nuklear::PropertyFloat("Far", perspectiveFar);
				if (perspectiveFar != newFar)
					camera.SetPerspectiveFarClip(newFar);
			}
		});

		DrawComponentNuklear<SpriteRendererComponent>("Color", entity, [](auto& component) {
			Nuklear::SetDynamicLayout(Nuklear::color_height);
			Nuklear::ColorEdit(component.Color);
		});
	}
}
