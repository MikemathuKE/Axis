#include <Axis.h>

class ExampleLayer : public Axis::Layer
{
public:
    ExampleLayer()
        :Layer("Example")
    {}

    void OnUpdate() override
    {
    }

    void OnEvent(Axis::Event& event)
    {
        if (event.GetEventType() == Axis::EventType::KeyPressed)
        {
            Axis::KeyPressedEvent& e = (Axis::KeyPressedEvent&)event;
            AXIS_TRACE("{0}", (char)e.GetKeyCode());
        }
    }
};

class SandBox : public Axis::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
        PushLayer(new Axis::ImGuiLayer());
    }
    ~SandBox()
    {
    }
};

Axis::Application* Axis::CreateApplication()
{
    return new SandBox();
}
