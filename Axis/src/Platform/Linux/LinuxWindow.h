#pragma once

#include "Axis/Core/Window.h"
#include "Axis/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Axis {

	class LinuxWindow : public Window
	{
	public:
		LinuxWindow(const WindowProps& props);
		virtual ~LinuxWindow();

		void OnUpdate() override;

		uint32_t GetWidth() const override { return m_Data.Width; }
		uint32_t GetHeight() const override { return m_Data.Height; }
		bool IsVSync() const override { return m_Data.VSync; }
		bool IsResizable() const override { return m_Data.Resizable; }

		virtual void* GetNativeWindow() override { return m_Window; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		void SetWindowMode(const WindowMode& mode, unsigned int width, unsigned int height) override;
		void SetIcon(const std::string& path) override;

	protected:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	protected:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		GLFWmonitor* m_PrimaryMonitor;
		GLFWvidmode m_VideoMode;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync, Resizable;

			WindowMode Mode;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;

		struct WindowedModeParams {
			uint32_t Width, Height;
			int32_t XPos, YPos;
		};
		WindowedModeParams m_OldWindowedParams;
	};

}
