#include "EditorLayer.h"

#include <imgui/imgui.h>

#define NK_INCLUDE_STANDARD_VARARGS
#include <Nuklear/nuklear.h>
#include <Axis/Nuklear/NuklearUtils.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Axis {

    static const char* s_MapTiles =
        "WWWWWWWWWWWWWWWWWWWWWWWW"
        "WWWWWWWWWDDDDWWWWWWWWWWW"
        "WWWWWWWDDDDDDDWWWWWWWWWW"
        "WWWWWDDDDDDDDDDWWWWWWWWW"
        "WWWWDDDDDDDDDDDDWWWWWWWW"
        "WWWDDDWWWDDDDDDDDWWWWWWW"
        "WWWDDDWWWDDDDDDDDDWWWWWW"
        "WWWDDDDDDDDDDDDDDDWWWWWW"
        "WWWWDDDDDDDDDDDDDWWWWWWW"
        "WWWWWDDDDDDDDDDDWWWWKWWW"
        "WWWWWWDDDDDDDDWWWWWWWWWW"
        "WWWWWWWDDDDDDWWWWWWWWWWW"
        "WWWWWWWWDDDDWWWWWWWWWWWW"
        "WWWWWWWWWDDWWWWWWWWWWWWW"
        "WWWWWWWWWWWWWWWWWWWWWWWW";

    EditorLayer::EditorLayer()
        :Layer(), m_CameraController(16.0f / 9.0f), m_MapWidth(24), m_MapHeight(15)
    {
    }

    void EditorLayer::OnAttach()
    {
        AXIS_PROFILE_FUNCTION();

        m_Texture = Texture2D::Create("assets/textures/AxisLogo.png");
        m_SpriteSheet = Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
        m_TextureStairs = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 11 }, { 128, 128 });
        m_TextureBarrel = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 1 }, { 128, 128 });
        m_TextureTree = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

        s_TextureMap['D'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 });
        s_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 });

        FrameBufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);

        m_CameraController.SetZoomLevel(5.0f);

        m_ActiveScene = CreateRef<Scene>();
        
        Entity square =  m_ActiveScene->CreateEntity("square");

        square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });
        m_SquareEntity = square;
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        AXIS_PROFILE_FUNCTION();

        //AXIS_TRACE("Frame rate: {0}fps", 1 / (float)ts);

        if (m_ViewportFocused)
            m_CameraController.OnUpdate(ts);
        Renderer2D::ResetStats();

        RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
        m_FrameBuffer->Bind();
        RenderCommand::Clear();


        Renderer2D::BeginScene(m_CameraController.GetCamera());

        // Update Scene
        m_ActiveScene->OnUpdate(ts);

        Renderer2D::EndScene();
        m_FrameBuffer->UnBind();
    }

    void EditorLayer::OnGUIRender()
    {
        AXIS_PROFILE_FUNCTION();

        auto stats = Renderer2D::GetStats();
        auto gui = GUILayer::GetBackend();

        switch (gui)
        {
        case GUIBackend::ImGui:
        {
            static bool dockspace_open = true;
            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->GetWorkPos());
                ImGui::SetNextWindowSize(viewport->GetWorkSize());
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            if (!opt_padding)
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    if (ImGui::MenuItem("Exit")) Application::Get().Close();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::Begin("Settings");

            ImGui::Text("Renderer2DStats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

            if (m_SquareEntity) {
                ImGui::Separator();
                auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
                ImGui::Text("%s", tag.c_str());

                auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
                ImGui::ColorEdit4("TriangleColor", glm::value_ptr(squareColor));
                ImGui::Separator();
            }

            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Viewport");

            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetGUILayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
            {
                m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
            }
            uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
            ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::End();
            break;
        }
        case GUIBackend::Nuklear:
        {
            struct nk_context* ctx = NuklearLayer::GetContext();
            if (nk_begin(ctx, "Menu", { 0, 0, 200, 25 }, NK_WINDOW_DOCK_MENU))
            {
                nk_layout_row_static(ctx, 20, 45, 1);
                if (nk_menu_begin_label(ctx, "MENU", NK_TEXT_CENTERED, nk_vec2(120, 200)))
                {
                    nk_layout_row_dynamic(ctx, 25, 1);
                    if (nk_menu_item_label(ctx, "Exit", NK_TEXT_LEFT))
                        Application::Get().Close();
                    nk_menu_end(ctx);
                }
            }
            nk_end(ctx);

            if (nk_begin(ctx, "Settings", { 25, 25, 250, 250 }, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
            {
                if (m_SquareEntity) {
                    nk_layout_row_dynamic(ctx, 20, 1);
                    auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
                    nk_label(ctx, tag.c_str(), NK_TEXT_CENTERED);

                    nk_colorf color;
                    auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
                    color << squareColor;                    
                    if (nk_combo_begin_color(ctx, nk_rgb_cf(color), nk_vec2(nk_widget_width(ctx), 400))) {
                        nk_layout_row_dynamic(ctx, 120, 1);
                        color = nk_color_picker(ctx, color, NK_RGBA);
                        nk_layout_row_dynamic(ctx, 25, 1);
                        color.r = nk_propertyf(ctx, "#R:", 0, color.r, 1.0f, 0.01f, 0.005f);
                        color.g = nk_propertyf(ctx, "#G:", 0, color.g, 1.0f, 0.01f, 0.005f);
                        color.b = nk_propertyf(ctx, "#B:", 0, color.b, 1.0f, 0.01f, 0.005f);
                        color.a = nk_propertyf(ctx, "#A:", 0, color.a, 1.0f, 0.01f, 0.005f);
                        nk_combo_end(ctx);
                    }
                    squareColor << color;
                }

                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "Renderer2D Stats:", NK_TEXT_CENTERED);
                nk_labelf(ctx, NK_TEXT_LEFT, "Draw Calls: %d", stats.DrawCalls);
                nk_labelf(ctx, NK_TEXT_LEFT, "Quads: %d", stats.QuadCount);
                nk_labelf(ctx, NK_TEXT_LEFT, "Vertices: %d", stats.GetTotalVertexCount());
                nk_labelf(ctx, NK_TEXT_LEFT, "Indices: %d", stats.GetTotalIndexCount());
            }
            nk_end(ctx);

            if (nk_begin(ctx, "Viewport", { 25, 25, 1280, 720 }, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR))
            {
                m_ViewportFocused = nk_window_has_focus(ctx);
                m_ViewportHovered = nk_window_is_hovered(ctx);
                Application::Get().GetGUILayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

                auto viewportPanelSize = nk_window_get_content_region_size(ctx);
                if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
                {
                    m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
                    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
                }

                nk_layout_row_static(ctx, (float)m_ViewportSize.y, (int)m_ViewportSize.x, 1);
                nk_image(ctx, nk_image_id(m_FrameBuffer->GetColorAttachmentRendererID()), nk_true);
            }
            nk_end(ctx);
            break;
        }
        }
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_CameraController.OnEvent(e);
    }

}