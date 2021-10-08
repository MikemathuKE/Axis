#include "Sandbox2D.h"

#include <imgui/imgui.h>
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
    
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Axis::Timestep ts)
{
    Axis::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
    Axis::RenderCommand::Clear();

    m_CameraController.OnUpdate(ts);

    Axis::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Axis::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor );
    Axis::Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, { 0.75f, 0.5f }, 90.0f, { 0.2f, 0.3f, 0.8f, 1.0f });

    Axis::Renderer2D::EndScene();
}

void Sandbox2D::OnGUIRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("TriangleColor", glm::value_ptr(m_SquareColor));
    ImGui::End();

    struct nk_context* ctx = Axis::NuklearLayer::GetContext();
    if (nk_begin(ctx, "Settings", { 25, 25, 250, 250 }, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
        NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
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
    }
    nk_end(ctx);
}

void Sandbox2D::OnEvent(Axis::Event& e)
{
	m_CameraController.OnEvent(e);
}