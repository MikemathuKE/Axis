#include "EditorLayer.h"

#include <imgui/imgui.h>

#define NK_INCLUDE_STANDARD_VARARGS
#include <glm/glm.hpp>
#include <Nuklear/nuklear.h>
#include <Axis/Nuklear/NuklearUtils.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Axis/Scene/SceneSerializer.h"
#include "Axis/utils/PlatformUtils.h"

namespace Axis {

    EditorLayer::EditorLayer()
        :Layer(), m_CameraController(16.0f / 9.0f)
    {
    }

    void EditorLayer::OnAttach()
    {
        AXIS_PROFILE_FUNCTION();

        FrameBufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);

        m_ActiveScene = CreateRef<Scene>();

        #if 0
        auto blueSquare = m_ActiveScene->CreateEntity("Blue Square");
        blueSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

        auto greenSquare = m_ActiveScene->CreateEntity("Green Square");
        greenSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

        auto redSquare = m_ActiveScene->CreateEntity("Red Square");
        redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

        auto model = m_ActiveScene->CreateEntity("Model");
        model.AddComponent<ModelComponent>("assets/models/cube/crate.fbx");

        auto cameraEntity = m_ActiveScene->CreateEntity("Camera A");
        cameraEntity.AddComponent<CameraComponent>().Primary = true;

        auto secondCamera = m_ActiveScene->CreateEntity("Camera B");
        secondCamera.AddComponent<CameraComponent>();

        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {
                auto& tc = GetComponent<TransformComponent>();
                tc.Translation.x = rand() % 10 - 5.0f;
            }

            void OnDestroy()
            {
            }

            void OnUpdate(Timestep ts)
            {
                auto& tc = GetComponent<TransformComponent>();
                float speed = 5.0f;

                if (Input::IsKeyPressed(KeyCode::A))
                    tc.Translation.x -= speed * ts;
                if (Input::IsKeyPressed(KeyCode::D))
                    tc.Translation.x += speed * ts;
                if (Input::IsKeyPressed(KeyCode::W))
                    tc.Translation.y += speed * ts;
                if (Input::IsKeyPressed(KeyCode::S))
                    tc.Translation.y -= speed * ts;
            }
        private:
            float rotation = 0.0f;
        };

        secondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        #endif

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        AXIS_PROFILE_FUNCTION();

        //AXIS_TRACE("Frame rate: {0}fps", 1 / (float)ts);
        if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && //zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);            
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            AXIS_INFO("Resizing Controllers");
        }


        if (m_ViewportFocused)
            m_CameraController.OnUpdate(ts);
        Renderer2D::ResetStats();

        RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
        m_FrameBuffer->Bind();
        RenderCommand::Clear();

        // Update Scene
        m_ActiveScene->OnUpdate(ts);

        m_FrameBuffer->UnBind();
    }

    void EditorLayer::OnGUIRender()
    {
        AXIS_PROFILE_FUNCTION();

        switch (GUILayer::GetBackend())
        {
        case GUIBackend::ImGui: OnImGuiRender(); break;
        case GUIBackend::Nuklear: OnNuklearRender(); break;
        }
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_CameraController.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(AXIS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        switch (e.GetKeyCode())
        {
        case Key::N:
        {
            if (control)
                NewScene();
            break;
        }
        case Key::O:
        {
            if (control)
                OpenScene();
            break;
        }
        case Key::S:
        {
            if (control && shift)
                SaveSceneAs();
            break;
        }
        }
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OpenScene()
    {
        std::string filePath = FileDialogs::OpenFile("Axis Scene (*.axis)\0*.axis\0");

        if (!filePath.empty()) {

            NewScene();
            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filePath);
            m_FrameBuffer->Resize(m_ViewportSize.x - 1.0f, m_ViewportSize.y - 1.0f);
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filePath = FileDialogs::SaveFile("Axis Scene (*.axis)\0*.axis\0");
        if (!filePath.empty()) {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filePath);
        }
    }

    void EditorLayer::OnImGuiRender()
    {
        auto stats = Renderer2D::GetStats();

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
        ImGui::Begin("DockSpace", &dockspace_open, window_flags);
        {
            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            ImGuiStyle& style = ImGui::GetStyle();
            float minWinSizeX = style.WindowMinSize.x;
            style.WindowMinSize.x = 370.0f;

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            style.WindowMinSize.x = minWinSizeX;

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.

                    if (ImGui::MenuItem("New", "Ctrl+N"))
                    {
                        NewScene();
                    }

                    if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    {
                        OpenScene();
                    }

                    if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    {
                        SaveSceneAs();
                    }

                    if (ImGui::MenuItem("Exit")) Application::Get().Close();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            m_SceneHierarchyPanel.OnImGuiRender();

            ImGui::Begin("Settings");
            {
                ImGui::Text("Renderer2DStats:");
                ImGui::Text("Draw Calls: %d", stats.DrawCalls);
                ImGui::Text("Quads: %d", stats.QuadCount);
                ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
                ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
            }
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Viewport");
            {

                m_ViewportFocused = ImGui::IsWindowFocused();
                m_ViewportHovered = ImGui::IsWindowHovered();
                Application::Get().GetGUILayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

                ImVec2 viewportPanelSize =                       ImGui::GetContentRegionAvail();
                m_ViewportSize = { viewportPanelSize.x,          viewportPanelSize.y };

                uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
                ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void EditorLayer::OnNuklearRender()
    {
        auto stats = Renderer2D::GetStats();
        struct nk_context* ctx = NuklearLayer::GetContext();

        if (Nuklear::Begin("Menu", { 0, 0, 200, 25 }, NK_WINDOW_DOCK_MENU))
        {
            Nuklear::SetStaticLayout(45);
            if (Nuklear::MenuLabelBegin("Menu", NK_TEXT_CENTERED, nk_vec2(200, 200)))
            {
                Nuklear::SetDynamicLayout();

                if (Nuklear::MenuItemLabel("New"))
                {
                    NewScene();
                }

                if (Nuklear::MenuItemLabel("Open..."))
                {
                    OpenScene();
                }

                if (Nuklear::MenuItemLabel("Save As..."))
                {
                    SaveSceneAs();
                }

                if (Nuklear::MenuItemLabel("Exit"))
                    Application::Get().Close();
                Nuklear::MenuEnd();
            }
        }
        Nuklear::End();

        m_SceneHierarchyPanel.OnNuklearRender();

        if (Nuklear::Begin("Settings", { 25, 25, 250, 250 }, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE))
        {
            Nuklear::SetDynamicLayout();
            Nuklear::Label("Renderer2D Stats:", NK_TEXT_CENTERED);
            Nuklear::Labelf(NK_TEXT_LEFT, "Draw Calls: %d", stats.DrawCalls);
            Nuklear::Labelf(NK_TEXT_LEFT, "Quads: %d", stats.QuadCount);
            Nuklear::Labelf(NK_TEXT_LEFT, "Vertices: %d", stats.GetTotalVertexCount());
            Nuklear::Labelf(NK_TEXT_LEFT, "Indices: %d", stats.GetTotalIndexCount());
        }
        Nuklear::End();

        if (Nuklear::Begin("Viewport", { 25, 25, 1280, 720 }, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_NO_SCROLLBAR))
        {
            m_ViewportFocused = nk_window_has_focus(ctx);
            m_ViewportHovered = nk_window_is_hovered(ctx);
            Application::Get().GetGUILayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
            
            auto viewportPanelSize = nk_window_get_content_region_size(ctx);
            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

            Nuklear::SetStaticLayout((int)m_ViewportSize.x, 1, (float)m_ViewportSize.y);
            Nuklear::Image(nk_image_id(m_FrameBuffer->GetColorAttachmentRendererID()), nk_true);
        }
        Nuklear::End();
    }

}
