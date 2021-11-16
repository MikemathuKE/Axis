#pragma once

#include "Axis/Core/GUILayer.h"

#include "Axis/Events/KeyEvent.h"
#include "Axis/Events/MouseEvent.h"

struct nk_context;
namespace Axis {

	class NuklearLayer : public GUILayer
	{
	public:
		NuklearLayer();
		~NuklearLayer();

		static struct nk_context* GetContext();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		virtual void OnGUIRender() override;

		virtual void Begin() override;
		virtual void End() override;

		virtual void BlockEvents(bool block) override { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = false;
	};

}