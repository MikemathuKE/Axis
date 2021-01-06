#include <Axis.h>

class ExampleLayer : public Axis::Layer
{
public:
    ExampleLayer()
        :Layer("Example")
    {
        glm::vec3 vec = { 1, 2, 3 };
        AXIS_TRACE("{0}", vec);
    }

    void OnUpdate() override
    {
    }

    void OnEvent(Axis::Event& event)
    {
    }
};

class SandBox : public Axis::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
    }
    ~SandBox()
    {
    }
};

Axis::Application* Axis::CreateApplication()
{
    return new SandBox();
}
