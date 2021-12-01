#include "Sandbox3D.h"

#include <glm/gtc/matrix_transform.hpp>

Sandbox3D::Sandbox3D()
    :Layer(), m_CameraController(60.0f, 16.0f / 9.0f, 0.1f, 10000.0f)
{
}

void Sandbox3D::OnAttach()
{
    std::vector<Axis::Vertex3D> squareVertices = {
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f } },

        { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f } },
        { {0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },
        { {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
        { {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
        { {-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
        { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },

        { {-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
        { {-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
        { {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },
        { {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },
        { {-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
        { {-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },

        { {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
        { {0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
        { {0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },
        { {0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },
        { {0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
        { {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, { 1.0f, 0.0f }},

        { {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f} },
        { {0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f} },
        { {0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f} },
        { {0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f} },
        { {-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f} },
        { {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f} },

        { {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} },
        { {0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },
        { {0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
        { {0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
        { {-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
        { {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} }
    };

    std::vector<uint32_t> squareIndices = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
    };

    std::vector<Axis::TextureData> textures = {
        {Axis::Texture2D::Create("assets/textures/Container.png"), Axis::TextureType::Diffuse},
        {Axis::Texture2D::Create("assets/textures/Container_Spec.png"), Axis::TextureType::Specular}
    };

    Axis::Ref<Axis::VertexArray> VA = Axis::VertexArray::Create();

    Axis::Ref<Axis::VertexBuffer> VB;
    VB = Axis::VertexBuffer::Create((float*)squareVertices.data(), (uint32_t)squareVertices.size() * sizeof(Axis::Vertex3D));

    VB->SetLayout({
           { Axis::ShaderDataType::Float3, "a_Position" },
           { Axis::ShaderDataType::Float3, "a_Normal" },
           { Axis::ShaderDataType::Float2, "a_TexCoord" }
        });
    VA->AddVertexBuffer(VB);

    m_Mesh = Axis::Mesh::Create(squareVertices, squareIndices, textures);
    m_Model = Axis::Model::Create("assets/models/nano_textured/nanosuit.obj");

    m_LightShader = (Axis::Shader::Create("assets/shaders/Light.glsl"));
    m_FlatColorShader = (Axis::Shader::Create("assets/shaders/FlatColor.glsl"));
    m_SkyBoxShader = (Axis::Shader::Create("assets/shaders/CubeMap.glsl"));
    m_EnvironmentShader = (Axis::Shader::Create("assets/shaders/Environment.glsl"));

    m_CubeMap = Axis::TextureCube::Create({
        "assets/textures/skybox/right.jpg",
        "assets/textures/skybox/left.jpg",
        "assets/textures/skybox/top.jpg",
        "assets/textures/skybox/bottom.jpg",
        "assets/textures/skybox/front.jpg",
        "assets/textures/skybox/back.jpg"
    });
}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Axis::Timestep ts)
{
    m_CameraController.OnUpdate(ts);
    //AXIS_TRACE("Frame Rate: {0}", 1.0f / (float)ts);

    Axis::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
    Axis::RenderCommand::Clear();

    Axis::Renderer::BeginScene(m_CameraController.GetCamera(), m_CameraController.GetCamera().GetViewMatrix());

    m_LightShader->Bind();
    m_LightShader->SetFloat("material.shininess", 32.0f);

    m_LightShader->SetFloat3("light.ambient", { 0.2f, 0.2f, 0.2f });
    m_LightShader->SetFloat3("light.diffuse", { 0.5f, 0.5f, 0.5f });
    m_LightShader->SetFloat3("light.specular", { 1.0f, 1.0f, 1.0f });
    m_LightShader->SetFloat3("light.direction", -m_LightPosition);
    m_LightShader->SetFloat("light.constant", 1.0f);
    m_LightShader->SetFloat("light.linear", 0.09f);
    m_LightShader->SetFloat("light.quadratic", 0.032f);

    m_LightShader->SetFloat3("u_ViewPos", m_CameraController.GetCamera().GetPosition());
    m_Model->SetPosition({ 2, 0, -5 });
    m_Model->Draw(m_LightShader);

    m_FlatColorShader->Bind();
    m_FlatColorShader->SetFloat4("u_Color", { m_LightColor.x, m_LightColor.y, m_LightColor.z, 1.0f });
    m_Mesh->SetPosition(m_LightPosition);
    m_Mesh->Draw(m_FlatColorShader);

    m_SkyBoxShader->Bind();
    m_CubeMap->Bind(0);
    m_SkyBoxShader->SetInt("u_Skybox", 0);
    m_Mesh->SetPosition({0, 0, 0});
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1000, 1000, 1000));
    Axis::Renderer::Submit(m_SkyBoxShader, m_Mesh->GetVertexArray(), scale);

    m_EnvironmentShader->Bind();
    m_CubeMap->Bind(0);
    m_SkyBoxShader->SetInt("u_Skybox", 0);
    m_SkyBoxShader->SetFloat3("u_ViewPos", m_CameraController.GetCamera().GetPosition());
    m_Mesh->SetPosition({ 0, 0, -2 });
    m_Mesh->Draw(m_EnvironmentShader);

    Axis::Renderer::EndScene();
}

void Sandbox3D::OnGUIRender()
{

}

void Sandbox3D::OnEvent(Axis::Event& e)
{
    m_CameraController.OnEvent(e);
}