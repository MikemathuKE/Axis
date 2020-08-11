#pragma once

namespace Axis
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space               = 32,
		Apostrophe          = 39, /* ' */
		Comma               = 44, /* , */
		Minus               = 45, /* - */
		Period              = 46, /* . */
		Slash               = 47, /* / */

		D0                  = 48, /* 0 */
		D1                  = 49, /* 1 */
		D2                  = 50, /* 2 */
		D3                  = 51, /* 3 */
		D4                  = 52, /* 4 */
		D5                  = 53, /* 5 */
		D6                  = 54, /* 6 */
		D7                  = 55, /* 7 */
		D8                  = 56, /* 8 */
		D9                  = 57, /* 9 */

		Semicolon           = 59, /* ; */
		Equal               = 61, /* = */

		A                   = 65,
		B                   = 66,
		C                   = 67,
		D                   = 68,
		E                   = 69,
		F                   = 70,
		G                   = 71,
		H                   = 72,
		I                   = 73,
		J                   = 74,
		K                   = 75,
		L                   = 76,
		M                   = 77,
		N                   = 78,
		O                   = 79,
		P                   = 80,
		Q                   = 81,
		R                   = 82,
		S                   = 83,
		T                   = 84,
		U                   = 85,
		V                   = 86,
		W                   = 87,
		X                   = 88,
		Y                   = 89,
		Z                   = 90,

		LeftBracket         = 91,  /* [ */
		Backslash           = 92,  /* \ */
		RightBracket        = 93,  /* ] */
		GraveAccent         = 96,  /* ` */

		World1              = 161, /* non-US #1 */
		World2              = 162, /* non-US #2 */

		/* Function keys */
		Escape              = 256,
		Enter               = 257,
		Tab                 = 258,
		Backspace           = 259,
		Insert              = 260,
		Delete              = 261,
		Right               = 262,
		Left                = 263,
		Down                = 264,
		Up                  = 265,
		PageUp              = 266,
		PageDown            = 267,
		Home                = 268,
		End                 = 269,
		CapsLock            = 280,
		ScrollLock          = 281,
		NumLock             = 282,
		PrintScreen         = 283,
		Pause               = 284,
		F1                  = 290,
		F2                  = 291,
		F3                  = 292,
		F4                  = 293,
		F5                  = 294,
		F6                  = 295,
		F7                  = 296,
		F8                  = 297,
		F9                  = 298,
		F10                 = 299,
		F11                 = 300,
		F12                 = 301,
		F13                 = 302,
		F14                 = 303,
		F15                 = 304,
		F16                 = 305,
		F17                 = 306,
		F18                 = 307,
		F19                 = 308,
		F20                 = 309,
		F21                 = 310,
		F22                 = 311,
		F23                 = 312,
		F24                 = 313,
		F25                 = 314,

		/* Keypad */
		KP0                 = 320,
		KP1                 = 321,
		KP2                 = 322,
		KP3                 = 323,
		KP4                 = 324,
		KP5                 = 325,
		KP6                 = 326,
		KP7                 = 327,
		KP8                 = 328,
		KP9                 = 329,
		KPDecimal           = 330,
		KPDivide            = 331,
		KPMultiply          = 332,
		KPSubtract          = 333,
		KPAdd               = 334,
		KPEnter             = 335,
		KPEqual             = 336,

		LeftShift           = 340,
		LeftControl         = 341,
		LeftAlt             = 342,
		LeftSuper           = 343,
		RightShift          = 344,
		RightControl        = 345,
		RightAlt            = 346,
		RightSuper          = 347,
		Menu                = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define AXIS_KEY_SPACE           ::Axis::Key::Space
#define AXIS_KEY_APOSTROPHE      ::Axis::Key::Apostrophe    /* ' */
#define AXIS_KEY_COMMA           ::Axis::Key::Comma         /* , */
#define AXIS_KEY_MINUS           ::Axis::Key::Minus         /* - */
#define AXIS_KEY_PERIOD          ::Axis::Key::Period        /* . */
#define AXIS_KEY_SLASH           ::Axis::Key::Slash         /* / */
#define AXIS_KEY_0               ::Axis::Key::D0
#define AXIS_KEY_1               ::Axis::Key::D1
#define AXIS_KEY_2               ::Axis::Key::D2
#define AXIS_KEY_3               ::Axis::Key::D3
#define AXIS_KEY_4               ::Axis::Key::D4
#define AXIS_KEY_5               ::Axis::Key::D5
#define AXIS_KEY_6               ::Axis::Key::D6
#define AXIS_KEY_7               ::Axis::Key::D7
#define AXIS_KEY_8               ::Axis::Key::D8
#define AXIS_KEY_9               ::Axis::Key::D9
#define AXIS_KEY_SEMICOLON       ::Axis::Key::Semicolon     /* ; */
#define AXIS_KEY_EQUAL           ::Axis::Key::Equal         /* = */
#define AXIS_KEY_A               ::Axis::Key::A
#define AXIS_KEY_B               ::Axis::Key::B
#define AXIS_KEY_C               ::Axis::Key::C
#define AXIS_KEY_D               ::Axis::Key::D
#define AXIS_KEY_E               ::Axis::Key::E
#define AXIS_KEY_F               ::Axis::Key::F
#define AXIS_KEY_G               ::Axis::Key::G
#define AXIS_KEY_H               ::Axis::Key::H
#define AXIS_KEY_I               ::Axis::Key::I
#define AXIS_KEY_J               ::Axis::Key::J
#define AXIS_KEY_K               ::Axis::Key::K
#define AXIS_KEY_L               ::Axis::Key::L
#define AXIS_KEY_M               ::Axis::Key::M
#define AXIS_KEY_N               ::Axis::Key::N
#define AXIS_KEY_O               ::Axis::Key::O
#define AXIS_KEY_P               ::Axis::Key::P
#define AXIS_KEY_Q               ::Axis::Key::Q
#define AXIS_KEY_R               ::Axis::Key::R
#define AXIS_KEY_S               ::Axis::Key::S
#define AXIS_KEY_T               ::Axis::Key::T
#define AXIS_KEY_U               ::Axis::Key::U
#define AXIS_KEY_V               ::Axis::Key::V
#define AXIS_KEY_W               ::Axis::Key::W
#define AXIS_KEY_X               ::Axis::Key::X
#define AXIS_KEY_Y               ::Axis::Key::Y
#define AXIS_KEY_Z               ::Axis::Key::Z
#define AXIS_KEY_LEFT_BRACKET    ::Axis::Key::LeftBracket   /* [ */
#define AXIS_KEY_BACKSLASH       ::Axis::Key::Backslash     /* \ */
#define AXIS_KEY_RIGHT_BRACKET   ::Axis::Key::RightBracket  /* ] */
#define AXIS_KEY_GRAVE_ACCENT    ::Axis::Key::GraveAccent   /* ` */
#define AXIS_KEY_WORLD_1         ::Axis::Key::World1        /* non-US #1 */
#define AXIS_KEY_WORLD_2         ::Axis::Key::World2        /* non-US #2 */

/* Function keys */
#define AXIS_KEY_ESCAPE          ::Axis::Key::Escape
#define AXIS_KEY_ENTER           ::Axis::Key::Enter
#define AXIS_KEY_TAB             ::Axis::Key::Tab
#define AXIS_KEY_BACKSPACE       ::Axis::Key::Backspace
#define AXIS_KEY_INSERT          ::Axis::Key::Insert
#define AXIS_KEY_DELETE          ::Axis::Key::Delete
#define AXIS_KEY_RIGHT           ::Axis::Key::Right
#define AXIS_KEY_LEFT            ::Axis::Key::Left
#define AXIS_KEY_DOWN            ::Axis::Key::Down
#define AXIS_KEY_UP              ::Axis::Key::Up
#define AXIS_KEY_PAGE_UP         ::Axis::Key::PageUp
#define AXIS_KEY_PAGE_DOWN       ::Axis::Key::PageDown
#define AXIS_KEY_HOME            ::Axis::Key::Home
#define AXIS_KEY_END             ::Axis::Key::End
#define AXIS_KEY_CAPS_LOCK       ::Axis::Key::CapsLock
#define AXIS_KEY_SCROLL_LOCK     ::Axis::Key::ScrollLock
#define AXIS_KEY_NUM_LOCK        ::Axis::Key::NumLock
#define AXIS_KEY_PRINT_SCREEN    ::Axis::Key::PrintScreen
#define AXIS_KEY_PAUSE           ::Axis::Key::Pause
#define AXIS_KEY_F1              ::Axis::Key::F1
#define AXIS_KEY_F2              ::Axis::Key::F2
#define AXIS_KEY_F3              ::Axis::Key::F3
#define AXIS_KEY_F4              ::Axis::Key::F4
#define AXIS_KEY_F5              ::Axis::Key::F5
#define AXIS_KEY_F6              ::Axis::Key::F6
#define AXIS_KEY_F7              ::Axis::Key::F7
#define AXIS_KEY_F8              ::Axis::Key::F8
#define AXIS_KEY_F9              ::Axis::Key::F9
#define AXIS_KEY_F10             ::Axis::Key::F10
#define AXIS_KEY_F11             ::Axis::Key::F11
#define AXIS_KEY_F12             ::Axis::Key::F12
#define AXIS_KEY_F13             ::Axis::Key::F13
#define AXIS_KEY_F14             ::Axis::Key::F14
#define AXIS_KEY_F15             ::Axis::Key::F15
#define AXIS_KEY_F16             ::Axis::Key::F16
#define AXIS_KEY_F17             ::Axis::Key::F17
#define AXIS_KEY_F18             ::Axis::Key::F18
#define AXIS_KEY_F19             ::Axis::Key::F19
#define AXIS_KEY_F20             ::Axis::Key::F20
#define AXIS_KEY_F21             ::Axis::Key::F21
#define AXIS_KEY_F22             ::Axis::Key::F22
#define AXIS_KEY_F23             ::Axis::Key::F23
#define AXIS_KEY_F24             ::Axis::Key::F24
#define AXIS_KEY_F25             ::Axis::Key::F25

/* Keypad */
#define AXIS_KEY_KP_0            ::Axis::Key::KP0
#define AXIS_KEY_KP_1            ::Axis::Key::KP1
#define AXIS_KEY_KP_2            ::Axis::Key::KP2
#define AXIS_KEY_KP_3            ::Axis::Key::KP3
#define AXIS_KEY_KP_4            ::Axis::Key::KP4
#define AXIS_KEY_KP_5            ::Axis::Key::KP5
#define AXIS_KEY_KP_6            ::Axis::Key::KP6
#define AXIS_KEY_KP_7            ::Axis::Key::KP7
#define AXIS_KEY_KP_8            ::Axis::Key::KP8
#define AXIS_KEY_KP_9            ::Axis::Key::KP9
#define AXIS_KEY_KP_DECIMAL      ::Axis::Key::KPDecimal
#define AXIS_KEY_KP_DIVIDE       ::Axis::Key::KPDivide
#define AXIS_KEY_KP_MULTIPLY     ::Axis::Key::KPMultiply
#define AXIS_KEY_KP_SUBTRACT     ::Axis::Key::KPSubtract
#define AXIS_KEY_KP_ADD          ::Axis::Key::KPAdd
#define AXIS_KEY_KP_ENTER        ::Axis::Key::KPEnter
#define AXIS_KEY_KP_EQUAL        ::Axis::Key::KPEqual

#define AXIS_KEY_LEFT_SHIFT      ::Axis::Key::LeftShift
#define AXIS_KEY_LEFT_CONTROL    ::Axis::Key::LeftControl
#define AXIS_KEY_LEFT_ALT        ::Axis::Key::LeftAlt
#define AXIS_KEY_LEFT_SUPER      ::Axis::Key::LeftSuper
#define AXIS_KEY_RIGHT_SHIFT     ::Axis::Key::RightShift
#define AXIS_KEY_RIGHT_CONTROL   ::Axis::Key::RightControl
#define AXIS_KEY_RIGHT_ALT       ::Axis::Key::RightAlt
#define AXIS_KEY_RIGHT_SUPER     ::Axis::Key::RightSuper
#define AXIS_KEY_MENU            ::Axis::Key::Menu

