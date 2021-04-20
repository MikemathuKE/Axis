#include "axispch.h"
#include "Axis/Core/Application.h"

#include "Axis/Events/MouseEvent.h"
#include "Axis/Events/KeyEvent.h"
#include "Axis/Core/Log.h"

#include "Axis/Core/Input.h"
#include "Axis/Renderer/Renderer.h"

#include <glad/glad.h>

namespace Axis{

    Application* Application::s_Instance = nullptr;

    Application::Application()
        :m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        AXIS_CORE_ASSERT(!s_Instance, "Application already Exists!");
        s_Instance = this;

        m_Window = (Scope<Window>)Window::Create();
        m_Window->SetEventCallback(AXIS_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushLayer(m_ImGuiLayer);
        m_NuklearLayer = new NuklearLayer();
        PushLayer(m_NuklearLayer);

        m_VertexArray.reset(VertexArray::Create());

        float vertices[7 * 3] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
        };
        Ref<VertexBuffer> vbo;
        vbo.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            BufferLayout layout = {
               { ShaderDataType::Float3, "a_Position" },
               { ShaderDataType::Float4, "a_Color" }
            };

            vbo->SetLayout(layout);
        }
        m_VertexArray->AddVertexBuffer(vbo);

        uint32_t indices[3] = {
            0, 1, 2
        };
        Ref<IndexBuffer> ibo;
        ibo.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

        m_Shader = CreateScope<Shader>(vertexSrc, fragmentSrc);

        m_SquareVA.reset(VertexArray::Create());

        float squareVertices[3 * 4] = {
           -0.75f, -0.75f, 0.0f,
            0.75f, -0.75f, 0.0f,
            0.75f,  0.75f, 0.0f,
           -0.75f,  0.75f, 0.0f
        };

        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        squareVB->SetLayout({
               { ShaderDataType::Float3, "a_Position" }
         });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {
            0, 1, 2,
            2, 3, 0
        };
        Ref<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

        m_SquareShader = CreateScope<Shader>(squareVertexSrc, squareFragmentSrc);
    }

    Application::~Application()
    {

    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(AXIS_BIND_EVENT_FN(Application::OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
            m_Camera.SetRotationZ(45.0f);

            RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
            RenderCommand::Clear();

            Renderer::BeginScene(m_Camera);

            Renderer::Submit(m_SquareShader, m_SquareVA);
            Renderer::Submit(m_Shader, m_VertexArray);

            Renderer::EndScene();

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
            m_NuklearLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnGUIRender();
            m_NuklearLayer->End();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}
