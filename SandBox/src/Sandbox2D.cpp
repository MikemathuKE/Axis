#include "Sandbox2D.h"

#include <imgui/imgui.h>

#define NK_INCLUDE_STANDARD_VARARGS
#include <Nuklear/nuklear.h>
#include <Axis/Nuklear/NuklearUtils.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

Sandbox2D::Sandbox2D()
    :Layer(), m_CameraController(16.0f / 9.0f), m_MapWidth(24), m_MapHeight(15)
{
}

void Sandbox2D::OnAttach()
{
    AXIS_PROFILE_FUNCTION();

    m_Texture = Axis::Texture2D::Create("assets/textures/AxisLogo.png");
    m_SpriteSheet = Axis::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
    m_TextureStairs = Axis::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 11 }, { 128, 128 });
    m_TextureBarrel = Axis::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 1 }, { 128, 128 });
    m_TextureTree = Axis::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

    s_TextureMap['D'] = Axis::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11}, {128, 128});
    s_TextureMap['W'] = Axis::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11}, {128, 128});

    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 5.0f;
    m_Particle.Velocity = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 3.0f, 1.0f };
    m_Particle.Position = { 0.0f, 0.0f };

    m_CameraController.SetZoomLevel(5.0f);
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Axis::Timestep ts)
{
    AXIS_PROFILE_FUNCTION();

    //AXIS_TRACE("Frame rate: {0}fps", 1 / (float)ts);

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

    for (uint32_t y = 0; y < m_MapHeight; y++) {
        for (uint32_t x = 0; x < m_MapWidth; x++) {
            char tileType = s_MapTiles[x + y * m_MapWidth];
            Axis::Ref<Axis::SubTexture2D> texture;
            if (s_TextureMap.find(tileType) != s_TextureMap.end())
                texture = s_TextureMap[tileType];
            else
                texture = m_TextureBarrel;
            Axis::Renderer2D::DrawQuad({ x-m_MapWidth/2.0f, y-m_MapHeight/2.0f, 1.0f }, { 1.0f, 1.0f }, texture);
        }
    }

    //Axis::Renderer2D::DrawQuad({ -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, m_TextureStairs);
    //Axis::Renderer2D::DrawQuad({ 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, m_TextureBarrel);
    //Axis::Renderer2D::DrawQuad({ 1.0f, 0.0f, 1.0f }, { 1.0f, 2.0f }, m_TextureTree);
    Axis::Renderer2D::EndScene();
}

void Sandbox2D::OnGUIRender()
{
    AXIS_PROFILE_FUNCTION();

    auto stats = Axis::Renderer2D::GetStats();
    switch (Axis::GUILayer::GetBackend()) {
    case Axis::GUIBackend::ImGui:
    {
        ImGui::Begin("Settings");

        ImGui::Text("Renderer2DStats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

        ImGui::ColorEdit4("TriangleColor", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }
    break;
    }
}

void Sandbox2D::OnEvent(Axis::Event& e)
{
	m_CameraController.OnEvent(e);
}