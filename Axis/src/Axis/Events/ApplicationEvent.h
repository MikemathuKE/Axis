#pragma once

#include "Axis/Events/Event.h"

namespace Axis{

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height)
            :m_Width(width), m_Height(height)
        {}

        inline uint32_t GetWidth() const { return m_Width; }
        inline uint32_t GetHeight() const { return m_Height; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizedEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        uint32_t m_Width, m_Height;
    };

    class WindowMovedEvent : public Event
	{
	public:
	    WindowMovedEvent(int32_t xPos, int32_t yPos)
			: m_XPos(xPos), m_YPos(yPos)
        {}

		inline int32_t GetXPos() const { return m_XPos; }
		inline int32_t GetYPos() const { return m_YPos; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << m_XPos << ", " << m_YPos;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int32_t m_XPos, m_YPos;
	};

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class WindowFocusedEvent : public Event
	{
	public:
		WindowFocusedEvent() = default;

		EVENT_CLASS_TYPE(WindowFocused)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowFocusLostEvent : public Event
	{
	public:
		WindowFocusLostEvent() = default;

		EVENT_CLASS_TYPE(WindowFocusLost)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowIconifiedEvent : public Event
	{
	public:
	    WindowIconifiedEvent() = default;

	    EVENT_CLASS_TYPE(WindowIconified)
	    EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowRestoredEvent : public Event
	{
	public:
	    WindowRestoredEvent() = default;

	    EVENT_CLASS_TYPE(WindowRestored)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

    class AppTickEvent : public Event
    {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

}
