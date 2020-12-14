#pragma once

#include "Axis/Core/Window.h"

#include <GLFW/glfw3.h>
	
namespace Axis {
	
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		uint32_t GetWidth() const override { return m_Data.Width; }
		uint32_t GetHeight() const override { return m_Data.Height; }
		bool IsVSync() const override { return m_Data.VSync; }
		bool IsResizable() const override { return m_Data.Resizable; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		void SetWindowMode(const WindowMode& mode, unsigned int width, unsigned int height) override;
		void SetIcon(const std::string& path) override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
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