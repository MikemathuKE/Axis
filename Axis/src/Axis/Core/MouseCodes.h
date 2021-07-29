#pragma once

namespace Axis
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0                = 0,
		Button1                = 1,
		Button2                = 2,
		Button3                = 3,
		Button4                = 4,
		Button5                = 5,
		Button6                = 6,
		Button7                = 7,

		ButtonLast             = Button7,
		ButtonLeft             = Button0,
		ButtonRight            = Button1,
		ButtonMiddle           = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define AXIS_MOUSE_BUTTON_0      ::Axis::Mouse::Button0
#define AXIS_MOUSE_BUTTON_1      ::Axis::Mouse::Button1
#define AXIS_MOUSE_BUTTON_2      ::Axis::Mouse::Button2
#define AXIS_MOUSE_BUTTON_3      ::Axis::Mouse::Button3
#define AXIS_MOUSE_BUTTON_4      ::Axis::Mouse::Button4
#define AXIS_MOUSE_BUTTON_5      ::Axis::Mouse::Button5
#define AXIS_MOUSE_BUTTON_6      ::Axis::Mouse::Button6
#define AXIS_MOUSE_BUTTON_7      ::Axis::Mouse::Button7
#define AXIS_MOUSE_BUTTON_LAST   ::Axis::Mouse::ButtonLast
#define AXIS_MOUSE_BUTTON_LEFT   ::Axis::Mouse::ButtonLeft
#define AXIS_MOUSE_BUTTON_RIGHT  ::Axis::Mouse::ButtonRight
#define AXIS_MOUSE_BUTTON_MIDDLE ::Axis::Mouse::ButtonMiddle
