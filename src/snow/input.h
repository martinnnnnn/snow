#pragma once

#include <windows.h>
#include <unordered_map>

#include "types.h"

namespace snow
{
	struct InputSystem
	{
		enum class KeyCode : u32
		{
			Mouse_LBUTTON = 0,
			Mouse_RBUTTON,
			Mouse_CANCEL,
			Mouse_MBUTTON,
			Mouse_XBUTTON1,
			Mouse_XBUTTON2,
			KB_BACK,
			KB_TAB,
			KB_CLEAR,
			KB_RETURN,
			KB_SHIFT,
			KB_CONTROL,
			KB_MENU,
			KB_PAUSE,
			KB_CAPITAL,
			KB_ESCAPE,
			KB_SPACE,
			KB_PRIOR,
			KB_NEXT,
			KB_END,
			KB_HOME,
			KB_LEFT,
			KB_UP,
			KB_RIGHT,
			KB_DOWN,
			KB_SELECT,
			KB_PRINT,
			KB_EXECUTE,
			KB_SNAPSHOT,
			KB_INSERT,
			KB_DELETE,
			KB_HELP,
			KB_0,
			KB_1,
			KB_2,
			KB_3,
			KB_4,
			KB_5,
			KB_6,
			KB_7,
			KB_8,
			KB_9,
			KB_A,
			KB_B,
			KB_C,
			KB_D,
			KB_E,
			KB_F,
			KB_G,
			KB_H,
			KB_I,
			KB_J,
			KB_K,
			KB_L,
			KB_M,
			KB_N,
			KB_O,
			KB_P,
			KB_Q,
			KB_R,
			KB_S,
			KB_T,
			KB_U,
			KB_V,
			KB_W,
			KB_X,
			KB_Y,
			KB_Z,
			KB_LWIN,
			KB_RWIN,
			KB_NUMPAD0,
			KB_NUMPAD1,
			KB_NUMPAD2,
			KB_NUMPAD3,
			KB_NUMPAD4,
			KB_NUMPAD5,
			KB_NUMPAD6,
			KB_NUMPAD7,
			KB_NUMPAD8,
			KB_NUMPAD9,
			KB_MULTIPLY,
			KB_ADD,
			KB_SEPARATOR,
			KB_SUBTRACT,
			KB_DECIMAL,
			KB_DIVIDE,
			KB_F1,
			KB_F2,
			KB_F3,
			KB_F4,
			KB_F5,
			KB_F6,
			KB_F7,
			KB_F8,
			KB_F9,
			KB_F10,
			KB_F11,
			KB_F12,
			KB_LSHIFT,
			KB_RSHIFT,
			KB_LCONTROL,
			KB_RCONTROL,
			KB_LMENU,
			KB_RMENU,

			ENUM_END
		};

		static constexpr u32 VK_KEYCODES[] =
		{
			VK_LBUTTON,		// Left mouse button
			VK_RBUTTON,		// Right mouse button
			VK_CANCEL,		// Control - break processing
			VK_MBUTTON,		// Middle mouse button(three - button mouse)
			VK_XBUTTON1,	// X1 mouse button
			VK_XBUTTON2,	// X2 mouse button
			VK_BACK,		// BACKSPACE key
			VK_TAB,			// TAB key
			VK_CLEAR,		// CLEAR key
			VK_RETURN,		// ENTER key
			VK_SHIFT,		// SHIFT key
			VK_CONTROL,		// CTRL key
			VK_MENU,		// ALT key
			VK_PAUSE,		// PAUSE key
			VK_CAPITAL,		// CAPS LOCK key
			VK_ESCAPE,		// ESC key
			VK_SPACE,		// SPACEBAR
			VK_PRIOR,		// PAGE UP key
			VK_NEXT,		// PAGE DOWN key
			VK_END,			// END key
			VK_HOME,		// HOME key
			VK_LEFT,		// LEFT ARROW key
			VK_UP,			// UP ARROW key
			VK_RIGHT,		// RIGHT ARROW key
			VK_DOWN,		// DOWN ARROW key
			VK_SELECT,		// SELECT key
			VK_PRINT,		// PRINT key
			VK_EXECUTE,		// EXECUTE key
			VK_SNAPSHOT,	// PRINT SCREEN key
			VK_INSERT,		// INS key
			VK_DELETE,		// DEL key
			VK_HELP,		// HELP key
			0x30,			// 0
			0x31,			// 1
			0x32,			// 2
			0x33,			// 3
			0x34,			// 4
			0x35,			// 5
			0x36,			// 6
			0x37,			// 7
			0x38,			// 8
			0x39,			// 9
			0x41,			// A
			0x42,			// B
			0x43,			// C
			0x44,			// D
			0x45,			// E
			0x46,			// F
			0x47,			// G
			0x48,			// H
			0x49,			// I
			0x4A,			// J
			0x4B,			// K
			0x4C,			// L
			0x4D,			// M
			0x4E,			// N
			0x4F,			// O
			0x50,			// P
			0x51,			// Q
			0x52,			// R
			0x53,			// S
			0x54,			// T
			0x55,			// U
			0x56,			// V
			0x57,			// W
			0x58,			// X
			0x59,			// Y
			0x5A,			// Z
			VK_LWIN,		// Left Windows key(Natural keyboard)
			VK_RWIN,		// Right Windows key(Natural keyboard)
			VK_NUMPAD0,		// Numeric keypad 0 key
			VK_NUMPAD1,		// Numeric keypad 1 key
			VK_NUMPAD2,		// Numeric keypad 2 key
			VK_NUMPAD3,		// Numeric keypad 3 key
			VK_NUMPAD4,		// Numeric keypad 4 key
			VK_NUMPAD5,		// Numeric keypad 5 key
			VK_NUMPAD6,		// Numeric keypad 6 key
			VK_NUMPAD7,		// Numeric keypad 7 key
			VK_NUMPAD8,		// Numeric keypad 8 key
			VK_NUMPAD9,		// Numeric keypad 9 key
			VK_MULTIPLY,	// Multiply key
			VK_ADD,			// Add key
			VK_SEPARATOR,	// Separator key
			VK_SUBTRACT,	// Subtract key
			VK_DECIMAL,		// Decimal key
			VK_DIVIDE,		// Divide key
			VK_F1,			// F1 key
			VK_F2,			// F2 key
			VK_F3,			// F3 key
			VK_F4,			// F4 key
			VK_F5,			// F5 key
			VK_F6,			// F6 key
			VK_F7,			// F7 key
			VK_F8,			// F8 key
			VK_F9,			// F9 key
			VK_F10,			// F10 key
			VK_F11,			// F11 key
			VK_F12,			// F12 key
			VK_LSHIFT,		// Left SHIFT key
			VK_RSHIFT,		// Right SHIFT key
			VK_LCONTROL,	// Left CONTROL key
			VK_RCONTROL,	// Right CONTROL key
			VK_LMENU,		// Left MENU key
			VK_RMENU		// Right MENU key
		};

		struct KeyInfo
		{
			KeyInfo(KeyCode code, u16 vk_code)
				: keycode(code)
				, vk_keycode(vk_code)
				, is_pressed(false)
				, was_pressed(false)
			{}

			KeyInfo()
				: keycode(KeyCode::ENUM_END)
				, vk_keycode(0)
				, is_pressed(false)
				, was_pressed(false)
			{}

			KeyCode	keycode;
			u16		vk_keycode;
			bool	is_pressed;
			bool	was_pressed;
		};

		std::unordered_map <KeyCode, KeyInfo> keys;

		void init()
		{
			for (u32 i = u32(KeyCode::Mouse_LBUTTON); i < u32(KeyCode::ENUM_END); ++i)
			{
				keys[(KeyCode)i] = KeyInfo((KeyCode)i, VK_KEYCODES[i]);
			}
		}

		void update()
		{
			for (auto it = keys.begin(); it != keys.end(); ++it)
			{
				KeyInfo& info = it->second;
				info.was_pressed = info.is_pressed;
				info.is_pressed = GetAsyncKeyState(info.vk_keycode) & 0x8000;
			}
		}

		bool pressed(KeyCode code)
		{
			return keys[code].is_pressed;
		}
		bool released(KeyCode code)
		{
			return !(keys[code].is_pressed);
		}

		bool just_pressed(KeyCode code)
		{
			return keys[code].is_pressed && !(keys[code].was_pressed);
		}

		bool just_released(KeyCode code)
		{
			return !(keys[code].is_pressed) && keys[code].was_pressed;
		}
	};
}

