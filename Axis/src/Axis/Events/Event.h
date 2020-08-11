#pragma once

#include "axispch.h"
#include "Axis/Core/Core.h"

namespace Axis{

    //Events in Axis are currently blocking. This means that when an event
    //occurs, it immediately gets dispatched and must be dealt with right away.
    //For the future, a better strategy might be to buffer events in an event
    //bus and process them during the "event" part of the update stage.

     enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocused, WindowFocusLost, WindowMoved, WindowIconified, WindowRestored,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication    = BIT(0),
        EventCategoryInput          = BIT(1),
        EventCategoryKeyboard       = BIT(2),
        EventCategoryMouse          = BIT(3),
        EventCategoryMouseButton    = BIT(4)
    };

    #define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::type; }\
                                    virtual EventType GetEventType() const override {return GetStaticType(); }\
                                    virtual const char* GetName() const override {return #type; }

    #define EVENT_CLASS_CATEGORY(category) virtual int32_t GetCategoryFlags() const override {return category; }

    class Event
    {
    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int32_t GetCategoryFlags() const = 0;
        virtual std::string ToString() const {return GetName();}

        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    public:
        bool Handled = false;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event &e)
            :m_Event(e)
        {}

        //F will be deduced by the compiler
        template <typename _T, typename _F>
        bool Dispatch(const _F& func)
        {
            if(m_Event.GetEventType() == _T::GetStaticType())
            {
                m_Event.Handled = func(static_cast<_T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;

    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }

}
