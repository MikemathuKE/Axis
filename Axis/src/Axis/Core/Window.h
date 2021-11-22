#pragma once

#include "axispch.h"

#include "Axis/Core/Base.h"
#include "Axis/Events/Event.h"

namespace Axis {

	enum class WindowMode
	{
		WINDOWED,
		FULLSCREEN,
		BORDERLESS
	};

	inline const char* ToString(WindowMode mode)
	{
		switch (mode)
		{
		case WindowMode::WINDOWED:
			return "Windowed";
		case WindowMode::FULLSCREEN:
			return "Full Screen";
		case WindowMode::BORDERLESS:
			return "Borderless";
		default:
			return "Null";
		}
	}

	struct WindowProps 
	{
		std::string Title;
		uint32_t Width, Height;
		bool Resizable;
		WindowMode Mode;

		WindowProps(const std::string& title = "Axis Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			WindowMode mode = WindowMode::WINDOWED,
			bool resizable = true)
			:Title(title), Width(width), Height(height), Mode(mode), Resizable(resizable)
		{
		}
	};

	// Interface representing a desktop system based window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetWindowMode(const WindowMode& mode, unsigned int width = 0, unsigned int height = 0) = 0;
		virtual void SetIcon(const std::string& path = "assets/textures/AxisLogo.png") = 0;

		virtual bool IsVSync() const = 0;
		virtual bool IsResizable() const = 0;

		virtual void* GetNativeWindow() = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}