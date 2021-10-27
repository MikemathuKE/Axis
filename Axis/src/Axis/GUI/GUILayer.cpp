#include "axispch.h"

#include "Axis/GUI/GUILayer.h"
#include "Axis/GUI/GUI.h"

namespace Axis {

	GUILayer::GUILayer()
		:Layer("GUILayer")
	{
	}

	GUILayer::~GUILayer()
	{
	}

	void GUILayer::OnAttach()
	{
		AXIS_PROFILE_FUNCTION();

		GUI::Init();
	}

	void GUILayer::OnDetach()
	{
		AXIS_PROFILE_FUNCTION();
	}

	void GUILayer::Begin()
	{
		AXIS_PROFILE_FUNCTION();
	}

	void GUILayer::OnGUIRender()
	{
		
	}

	void GUILayer::End()
	{

	}

}