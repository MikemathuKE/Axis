#pragma once

#include "Axis/Core/Layer.h"

namespace Axis {

    enum class GUIBackend
    {
        ImGui = 0,
        Nuklear
    };

    class GUILayer : public Layer
    {
    public:
        virtual ~GUILayer() {}

        virtual void Begin() = 0;
        virtual void End() = 0;
        virtual void BlockEvents(bool) = 0;

        static GUILayer* Create(GUIBackend gui);
        static GUIBackend GetBackend();
    };

}