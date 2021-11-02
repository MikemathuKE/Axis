#include "Sandbox3D.h"

#include <glm/gtc/matrix_transform.hpp>

Sandbox3D::Sandbox3D()
	:Layer(), m_CameraController(60.0f, 16.0f / 9.0f, 0.1f, 10.0f)
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

    m_Mesh = Axis::Mesh::Create(squareVertices, squareIndices, textures);
    m_Model = Axis::Model::Create("assets/models/nano_textured/nanosuit.obj");

    m_LightShader = (Axis::Shader::Create("assets/shaders/Light.glsl"));
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

    m_LightShader->Bind();
    m_LightShader->SetFloat("material.shininess", 32.0f);

    m_LightShader->SetFloat3("light.ambient", { 0.2f, 0.2f, 0.2f });
    m_LightShader->SetFloat3("light.diffuse", { 0.5f, 0.5f, 0.5f });
    m_LightShader->SetFloat3("light.specular", { 1.0f, 1.0f, 1.0f });
    m_LightShader->SetFloat3("light.position", m_LightPosition);

    m_LightShader->SetFloat3("u_ViewPos", m_CameraController.GetCamera().GetPosition());
    m_Model->Draw(m_LightShader);

    m_FlatColorShader->Bind();
    m_FlatColorShader->SetFloat4("u_Color", { m_LightColor.x, m_LightColor.y, m_LightColor.z, 1.0f });
    m_Mesh->SetPosition(m_LightPosition);
    m_Mesh->Draw(m_FlatColorShader);

    Axis::Renderer::EndScene();
}

void Sandbox3D::OnGUIRender()
{

}

void Sandbox3D::OnEvent(Axis::Event& e)
{
	m_CameraController.OnEvent(e);
}