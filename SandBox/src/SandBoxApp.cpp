#include <Axis.h>

class SandBox : public Axis::Application
{
public:
    SandBox(){}
    ~SandBox(){}
};

Axis::Application* Axis::CreateApplication()
{
    return new SandBox();
}
