#include <Axis.h>
#include <Axis/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Axis {

    class HazelEditor : public Application
    {
    public:
        HazelEditor()
            :Application("Axis Editor")
        {
            PushLayer(new EditorLayer());
        }
        ~HazelEditor()
        {
        }
    };

    Application* CreateApplication()
    {
        return new HazelEditor();
    }

}
