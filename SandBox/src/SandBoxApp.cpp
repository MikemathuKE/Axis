#include <Axis.h>

class ExampleLayer : public Axis::Layer
{
public:
    ExampleLayer()
        :Layer("Example")
    {}

    void OnUpdate() override
    {
        AXIS_INFO("ExampleLayer::Update");
    }

    void OnEvent(Axis::Event& event)
    {
        AXIS_TRACE("{0}", event);
    }
};

class SandBox : public Axis::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
    }
    ~SandBox() {}
};

Axis::Application* Axis::CreateApplication()
{
    return new SandBox();
}
