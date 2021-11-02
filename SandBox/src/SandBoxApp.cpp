#include <Axis.h>
#include <Axis/Core/EntryPoint.h>
 
#include <imgui/imgui.h>
#include <Nuklear/nuklear.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"
#include "Sandbox3D.h"


class ExampleLayer2D : public Axis::Layer
{
public:
    ExampleLayer2D()
        :Layer("Example"), m_CameraController(16.0f/9.0f, true)
    {
        m_VertexArray = Axis::VertexArray::Create();

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f, 
             0.0f,  0.5f, 0.0f, 
             0.5f, -0.5f, 0.0f
        };
        Axis::Ref<Axis::VertexBuffer> vbo;
        vbo = (Axis::VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            Axis::BufferLayout layout = {
               { Axis::ShaderDataType::Float3, "a_Position" }
            };

            vbo->SetLayout(layout);
        }
        m_VertexArray->AddVertexBuffer(vbo);

        uint32_t indices[3] = {
            0, 1, 2
        };
        Axis::Ref<Axis::IndexBuffer> ibo;
        ibo = (Axis::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(ibo);

        m_Shader = (Axis::Shader::Create("assets/shaders/FlatColor.glsl"));

        m_SquareVA = Axis::VertexArray::Create();

        float squareVertices[5 * 4] = {
           -0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
            0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
            0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
           -0.75f,  0.75f, 0.0f, 0.0f, 1.0f
        };

        Axis::Ref<Axis::VertexBuffer> squareVB;
        squareVB = (Axis::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        squareVB->SetLayout({
               { Axis::ShaderDataType::Float3, "a_Position" },
               { Axis::ShaderDataType::Float2, "a_TexCoord" }
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {
            0, 1, 2,
            2, 3, 0
        };
        Axis::Ref<Axis::IndexBuffer> squareIB;
        squareIB = (Axis::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        m_SquareShader = (Axis::Shader::Create("assets/shaders/Texture.glsl"));
        m_Texture = Axis::Texture2D::Create("assets/textures/Axis_Trans.png");
        m_SquareShader->Bind();
        m_SquareShader->SetInt("u_Texture", 0);
    }

    void OnUpdate(Axis::Timestep ts) override
    {
        Axis::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
        Axis::RenderCommand::Clear();

        m_CameraController.OnUpdate(ts);

        Axis::Renderer::BeginScene(m_CameraController.GetCamera());

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_SquarePosition);
        m_Texture->Bind();
        Axis::Renderer::Submit(m_SquareShader, m_SquareVA, translation);        

        m_Shader->Bind();
        m_Shader->SetFloat4("u_Color", m_TriangleColor);
        Axis::Renderer::Submit(m_Shader, m_VertexArray);

        Axis::Renderer::EndScene();
    }

    void OnGUIRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit4("TriangleColor", glm::value_ptr(m_TriangleColor));
        ImGui::End();

        struct nk_context* ctx = Axis::NuklearLayer::GetContext();
        if (nk_begin(ctx, "Settings", { 25, 25, 250, 250 }, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
            nk_colorf color;
            color.r = m_TriangleColor.r;
            color.g = m_TriangleColor.g;
            color.b = m_TriangleColor.b;
            color.a = m_TriangleColor.a;
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
            m_TriangleColor.r = color.r;
            m_TriangleColor.g = color.g;
            m_TriangleColor.b = color.b;
            m_TriangleColor.a = color.a;
        }
        nk_end(ctx);
    }

    void OnEvent(Axis::Event& e) override
    {
        m_CameraController.OnEvent(e);
    }

private:
    Axis::Ref<Axis::Shader> m_Shader;
    Axis::Ref<Axis::VertexArray> m_VertexArray;

    Axis::Ref<Axis::Shader> m_SquareShader;
    Axis::Ref<Axis::VertexArray> m_SquareVA;

    Axis::Ref<Axis::Texture2D> m_Texture;

    Axis::OrthographicCameraController m_CameraController;
    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_CameraSpeed = 5.0f;
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 180.0f;

    glm::vec3 m_SquarePosition = glm::vec3(0.0f, 0.0f, -0.1f);
    float m_SquareSpeed = 2.0f;

    glm::vec4 m_TriangleColor = { 0.9f, 0.8f, 0.1f, 1.0f };
};

class ExampleLayer3D : public Axis::Layer
{
public:
    ExampleLayer3D()
        :Layer("Example"), m_CameraController( 60.0f, 16.0f/9.0f, 0.1f, 10.0f)
    {
        m_SquareVA = (Axis::VertexArray::Create());
        float squareVertices[3 * 4 * 2] = {
            -1.0f, -1.0f, -1.0f,     -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,      1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,      1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,     -1.0f,  1.0f,  1.0f
        };

        Axis::Ref<Axis::VertexBuffer> squareVB;
        squareVB = (Axis::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        squareVB->SetLayout({
               { Axis::ShaderDataType::Float3, "a_Position" }
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6 * 6] = {
            0, 1, 2,    2, 3, 0,    0, 4, 7,    7, 1, 0,
            4, 5, 6,    6, 7, 4,    5, 3, 2,    2, 6, 5,
            7, 6, 2,    2, 1, 7,    0, 3, 5,    5, 4, 0
        };
        Axis::Ref<Axis::IndexBuffer> squareIB;
        squareIB = (Axis::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        m_SquareShader = (Axis::Shader::Create("assets/shaders/FlatColor.glsl"));
    }

    void OnUpdate(Axis::Timestep ts) override
    {
        m_CameraController.OnUpdate(ts);

        Axis::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
        Axis::RenderCommand::Clear();

        Axis::Renderer::BeginScene(m_CameraController.GetCamera());

        m_SquareShader->Bind();
        m_SquareShader->SetFloat4("u_Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        Axis::Renderer::Submit(m_SquareShader, m_SquareVA);

        Axis::Renderer::EndScene();
    }

    void OnEvent(Axis::Event& e)
    {
        m_CameraController.OnEvent(e);
    }

private:
    Axis::Ref<Axis::Shader> m_SquareShader;
    Axis::Ref<Axis::VertexArray> m_SquareVA;

    Axis::PerspectiveCameraController m_CameraController;
    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, -1.0f };
    float m_CameraSpeed = 5.0f;
    glm::vec3 m_CameraRotation = glm::vec3(0.0f);
    float m_CameraRotationSpeed = 180.0f;
};

class SandBox : public Axis::Application
{
public:
    SandBox()
    {
        //PushLayer(new ExampleLayer3D());
        PushLayer(new Sandbox3D());
    }
    ~SandBox()
    {
    }
};

Axis::Application* Axis::CreateApplication()
{
    return new SandBox();
}
