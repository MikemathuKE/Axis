#include "Sandbox3D.h"

Sandbox3D::Sandbox3D()
	:Layer(), m_CameraController(60.0f, 16.0f / 9.0f, 0.1f, 10.0f)
{
}

void Sandbox3D::OnAttach()
{
    m_VertexArray = (Axis::VertexArray::Create());
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
    m_VertexArray->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6 * 6] = {
        0, 1, 2,    2, 3, 0,    0, 4, 7,    7, 1, 0,
        4, 5, 6,    6, 7, 4,    5, 3, 2,    2, 6, 5,
        7, 6, 2,    2, 1, 7,    0, 3, 5,    5, 4, 0
    };
    Axis::Ref<Axis::IndexBuffer> squareIB;
    squareIB = (Axis::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_VertexArray->SetIndexBuffer(squareIB);

    m_FlatColorShader = (Axis::Shader::Create("assets/shaders/FlatColor.glsl"));
}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Axis::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    Axis::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
    Axis::RenderCommand::Clear();

    Axis::Renderer::BeginScene(m_CameraController.GetCamera());

    m_FlatColorShader->Bind();
    m_FlatColorShader->SetFloat4("u_Color", glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));
    Axis::Renderer::Submit(m_FlatColorShader, m_VertexArray);

    Axis::Renderer::EndScene();
}

void Sandbox3D::OnGUIRender()
{

}

void Sandbox3D::OnEvent(Axis::Event& e)
{
	m_CameraController.OnEvent(e);
}