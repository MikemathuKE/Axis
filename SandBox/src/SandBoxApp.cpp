#include <Axis.h>

class ExampleLayer2D : public Axis::Layer
{
public:
    ExampleLayer2D()
        :Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        m_VertexArray.reset(Axis::VertexArray::Create());

        float vertices[7 * 3] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
        };
        Axis::Ref<Axis::VertexBuffer> vbo;
        vbo.reset(Axis::VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            Axis::BufferLayout layout = {
               { Axis::ShaderDataType::Float3, "a_Position" },
               { Axis::ShaderDataType::Float4, "a_Color" }
            };

            vbo->SetLayout(layout);
        }
        m_VertexArray->AddVertexBuffer(vbo);

        uint32_t indices[3] = {
            0, 1, 2
        };
        Axis::Ref<Axis::IndexBuffer> ibo;
        ibo.reset(Axis::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(ibo);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            
            out vec3 v_Position;
            out vec4 v_Color;
            
            void main() 
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            in vec3 v_Position;
            in vec4 v_Color;
            
            void main() 
            {
                color = v_Color;
            }
        )";

        m_Shader = Axis::CreateScope<Axis::Shader>(vertexSrc, fragmentSrc);

        m_SquareVA.reset(Axis::VertexArray::Create());

        float squareVertices[3 * 4] = {
           -0.75f, -0.75f, 0.0f,
            0.75f, -0.75f, 0.0f,
            0.75f,  0.75f, 0.0f,
           -0.75f,  0.75f, 0.0f
        };

        Axis::Ref<Axis::VertexBuffer> squareVB;
        squareVB.reset(Axis::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        squareVB->SetLayout({
               { Axis::ShaderDataType::Float3, "a_Position" }
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {
            0, 1, 2,
            2, 3, 0
        };
        Axis::Ref<Axis::IndexBuffer> squareIB;
        squareIB.reset(Axis::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string squareVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            
            void main() 
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string squareFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            in vec3 v_Position;
            
            void main() 
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
            }
        )";

        m_SquareShader = Axis::CreateScope<Axis::Shader>(squareVertexSrc, squareFragmentSrc);
    }

    void OnUpdate() override
    {
        if (Axis::Input::IsKeyPressed(AXIS_KEY_A) )
            m_CameraPosition.x -= m_CameraSpeed;
        else if (Axis::Input::IsKeyPressed(AXIS_KEY_D))
            m_CameraPosition.x += m_CameraSpeed;

        if (Axis::Input::IsKeyPressed(AXIS_KEY_W))
            m_CameraPosition.y -= m_CameraSpeed;
        else if (Axis::Input::IsKeyPressed(AXIS_KEY_S))
            m_CameraPosition.y += m_CameraSpeed;

        if (Axis::Input::IsKeyPressed(AXIS_KEY_Q))
            m_CameraRotation += m_CameraRotationSpeed;
        if (Axis::Input::IsKeyPressed(AXIS_KEY_E))
            m_CameraRotation -= m_CameraRotationSpeed;


        Axis::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
        Axis::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Axis::Renderer::BeginScene(m_Camera);

        Axis::Renderer::Submit(m_SquareShader, m_SquareVA);
        Axis::Renderer::Submit(m_Shader, m_VertexArray);

        Axis::Renderer::EndScene();
    }

private:
    Axis::Ref<Axis::Shader> m_Shader;
    Axis::Ref<Axis::VertexArray> m_VertexArray;

    Axis::Ref<Axis::Shader> m_SquareShader;
    Axis::Ref<Axis::VertexArray> m_SquareVA;

    Axis::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_CameraSpeed = 0.01f;
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 2.0f;
};

class ExampleLayer3D : public Axis::Layer
{
public:
    ExampleLayer3D()
        :Layer("Example"), m_Camera( glm::radians(60.0f), 16.0f/9.0f, 1.0f, 10.0f)
    {
        m_SquareVA.reset(Axis::VertexArray::Create());
        float squareVertices[3 * 4 * 2] = {
            -1.0f, -1.0f, -1.0f,     -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,      1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,      1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,     -1.0f,  1.0f,  1.0f
        };

        Axis::Ref<Axis::VertexBuffer> squareVB;
        squareVB.reset(Axis::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

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
        squareIB.reset(Axis::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string squareVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            
            void main() 
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string squareFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            in vec3 v_Position;
            
            void main() 
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
            }
        )";

        m_SquareShader = Axis::CreateScope<Axis::Shader>(squareVertexSrc, squareFragmentSrc);
    }

    void OnUpdate() override
    {
        if (Axis::Input::IsKeyPressed(AXIS_KEY_A))
            m_CameraPosition.x -= m_CameraSpeed;
        else if (Axis::Input::IsKeyPressed(AXIS_KEY_D))
            m_CameraPosition.x += m_CameraSpeed;

        if (Axis::Input::IsKeyPressed(AXIS_KEY_W))
            m_CameraPosition.y -= m_CameraSpeed;
        else if (Axis::Input::IsKeyPressed(AXIS_KEY_S))
            m_CameraPosition.y += m_CameraSpeed;

        if (Axis::Input::IsKeyPressed(AXIS_KEY_UP))
            m_CameraPosition.z -= m_CameraSpeed;
        else if (Axis::Input::IsKeyPressed(AXIS_KEY_DOWN))
            m_CameraPosition.z += m_CameraSpeed;

        if (Axis::Input::IsKeyPressed(AXIS_KEY_Q))
            m_CameraRotation.z += m_CameraRotationSpeed;
        else if (Axis::Input::IsKeyPressed(AXIS_KEY_E))
            m_CameraRotation.z -= m_CameraRotationSpeed;

        if (Axis::Input::IsKeyPressed(AXIS_KEY_J))
            m_CameraRotation.y += m_CameraRotationSpeed;
        else if (Axis::Input::IsKeyPressed(AXIS_KEY_L))
            m_CameraRotation.y -= m_CameraRotationSpeed;

        if (Axis::Input::IsKeyPressed(AXIS_KEY_I))
            m_CameraRotation.x += m_CameraRotationSpeed;
        else if (Axis::Input::IsKeyPressed(AXIS_KEY_K))
            m_CameraRotation.x -= m_CameraRotationSpeed;


        Axis::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
        Axis::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Axis::Renderer::BeginScene(m_Camera);

        Axis::Renderer::Submit(m_SquareShader, m_SquareVA);

        Axis::Renderer::EndScene();
    }

private:
    Axis::Ref<Axis::Shader> m_SquareShader;
    Axis::Ref<Axis::VertexArray> m_SquareVA;

    Axis::PerspectiveCamera m_Camera;
    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 2.0f };
    float m_CameraSpeed = 0.01f;
    glm::vec3 m_CameraRotation = glm::vec3(0.0f);
    float m_CameraRotationSpeed = 2.0f;
};

class SandBox : public Axis::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer2D());
    }
    ~SandBox()
    {
    }
};

Axis::Application* Axis::CreateApplication()
{
    return new SandBox();
}
