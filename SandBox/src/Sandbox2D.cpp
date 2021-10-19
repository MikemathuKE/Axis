#include "Sandbox2D.h"

#include <imgui/imgui.h>

#define NK_INCLUDE_STANDARD_VARARGS
#include <Nuklear/nuklear.h>
#include <Axis/Nuklear/NuklearUtils.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    :Layer(), m_CameraController(16.0f / 9.0f)
{
}

void Sandbox2D::OnAttach()
{
    AXIS_PROFILE_FUNCTION();

    m_Texture = Axis::Texture2D::Create("assets/textures/AxisLogo.png");
    m_SpriteSheet = Axis::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
    m_TextureStairs = Axis::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128, 128 });
    m_TextureBarrel = Axis::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 2 }, { 128, 128 });
    m_TextureTree = Axis::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 5.0f;
    m_Particle.Velocity = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 3.0f, 1.0f };
    m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Axis::Timestep ts)
{
    AXIS_PROFILE_FUNCTION();

    AXIS_INFO("Frame rate: {0}", 1 / (float)ts);

    m_CameraController.OnUpdate(ts);
    Axis::Renderer2D::ResetStats();
    {
        AXIS_PROFILE_SCOPE("Renderer Prep");
        Axis::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
        Axis::RenderCommand::Clear();
    }
    #if 0
    {
        AXIS_PROFILE_SCOPE("Renderer Draw");
        Axis::Renderer2D::BeginScene(m_CameraController.GetCamera());

        Axis::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor);
        Axis::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 0.5f, 0.5f }, m_Texture, 2.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
        Axis::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_Texture);

        Axis::Renderer2D::EndScene();

        Axis::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f) {
            for (float x = -5.0f; x < 5.0f; x += 0.5f) {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                Axis::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        }
        Axis::Renderer2D::EndScene();

    }
    #endif


    if (Axis::Input::IsMouseButtonPressed(AXIS_MOUSE_BUTTON_LEFT))
    {
        auto [x, y] = Axis::Input::GetMousePosition();
        auto width = Axis::Application::Get().GetWindow().GetWidth();
        auto height = Axis::Application::Get().GetWindow().GetHeight();

        auto bounds = m_CameraController.GetBounds();
        auto pos = m_CameraController.GetCamera().GetPosition();
        x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
        y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
        m_Particle.Position = { x + pos.x, y + pos.y };
        for (int i = 0; i < 5; i++)
            m_ParticleSystem.Emit(m_Particle);
    }

    m_ParticleSystem.OnUpdate(ts);
    m_ParticleSystem.OnRender(m_CameraController.GetCamera());
    
    Axis::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Axis::Renderer2D::DrawQuad({ -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, m_TextureStairs);
    Axis::Renderer2D::DrawQuad({ 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, m_TextureBarrel);
    Axis::Renderer2D::DrawQuad({ 1.0f, 0.0f, 1.0f }, { 1.0f, 2.0f }, m_TextureTree);
    Axis::Renderer2D::EndScene();
}

void Sandbox2D::OnGUIRender()
{
    AXIS_PROFILE_FUNCTION();

    auto stats = Axis::Renderer2D::GetStats();

    ImGui::Begin("Settings");
    
    ImGui::Text("Renderer2DStats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit4("TriangleColor", glm::value_ptr(m_SquareColor));
    ImGui::End();

    struct nk_context* ctx = Axis::NuklearLayer::GetContext();
    if (nk_begin(ctx, "Settings", { 25, 25, 250, 250 }, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
        nk_colorf color;
        color << m_SquareColor;
        nk_layout_row_dynamic(ctx, 20, 1);
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
        m_SquareColor << color;
        
        nk_label(ctx, "Renderer2D Stats:", NK_TEXT_CENTERED);
        nk_labelf(ctx, NK_TEXT_LEFT, "Draw Calls: %d", stats.DrawCalls);
        nk_labelf(ctx, NK_TEXT_LEFT, "Quads: %d", stats.QuadCount);
        nk_labelf(ctx, NK_TEXT_LEFT, "Vertices: %d", stats.GetTotalVertexCount());
        nk_labelf(ctx, NK_TEXT_LEFT, "Indices: %d", stats.GetTotalIndexCount());
    }
    nk_end(ctx);
}

void Sandbox2D::OnEvent(Axis::Event& e)
{
	m_CameraController.OnEvent(e);
}