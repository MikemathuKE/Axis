#pragma once

#include "Axis/Core/Layer.h"

struct nk_context;
namespace Axis {

	class NuklearLayer : public Layer
	{
	public:
		NuklearLayer();
		~NuklearLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnGUIRender() override;

		void Begin();
		void End();
	protected:
		struct nk_context* m_Context;		
	};

}