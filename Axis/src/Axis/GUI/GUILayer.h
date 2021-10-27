#pragma once

#include "Axis/Core/Layer.h"

namespace Axis {

	class GUILayer : public Layer
	{
	public:
		GUILayer();
		~GUILayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnGUIRender() override;

		void Begin();
		void End();
	};

}