#include <Axis.h>
#include <Axis/Core/EntryPoint.h>

#include <imgui/imgui.h>
#include <Nuklear/nuklear.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"
#include "Sandbox3D.h"

class SandBox : public Axis::Application
{
public:
    SandBox()
        :Application("Sandbox", Axis::GUIBackend::ImGui)
    {
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
