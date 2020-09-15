#include "stdio.h"


#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <fmt/format.h>

#include "types.h"
#include "input.h"

void get_screen_resolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

struct Vec2
{
	i32 x;
	i32 y;
};

struct Cell
{
	Cell() : position(), value() {}
	Cell(Vec2 pos, WCHAR val) : position(pos), value(val) {}

	Vec2 position;
	WCHAR value;
};


struct DrawComponent
{
	Vec2 position;
	std::vector<Cell> cells;

	static void from_text(const std::string& text, DrawComponent& output)
	{
		output.cells.reserve(text.length());

		Vec2 position = output.position;
		for (i32 i = 0; i < text.length(); ++i)
		{
			if (text[i] == '\n')
			{
				position.x = output.position.x;
				position.y++;
				continue;
			}
			else if (text[i] == '\t')
			{
				position.x += 4;
				continue;
			}
			else
			{
				output.cells.emplace_back(position, text[i]);
				position.x++;
			}

		}
	}
};

struct KeybordInputComponent
{
	Vec2 position;
	Vec2 current_position;
	std::vector<Cell> cells;

	void handle_input(const WCHAR value)
	{
		if (value == '\n')
		{
			current_position.x = position.x;
			current_position.y++;
		}
		else if (value == '\t')
		{
			current_position.x += 4;
		}
		else
		{
			cells.emplace_back(current_position, value);
			current_position.x++;
		}
	}

	void erase_last()
	{
		Vec2 last_position = cells.back().position;
		if (current_position.x == last_position.x - 1 && current_position.y == last_position.y)
		{
			cells.pop_back();
			current_position = last_position;
		}
		else
		{
			current_position.x = last_position.x + 1;
			current_position.y = last_position.y;
		}
	}
};


struct Actor
{
	Vec2 position;
	DrawComponent draw_component;
};


struct ConsoleRenderer
{
	HWND handle;
	Vec2 screen_resolution;
	Vec2 resolution;
	Vec2 position;
	i32 columns_count;
	i32 rows_count;
	CHAR_INFO* framebuffer;
	i32 framebuffer_byte_size;
	HANDLE output_handle;

	void init(const Vec2& console_resolution = Vec2{-1, -1})
	{
		// 1. get console window
		handle = GetConsoleWindow();

		// 2. get screen info
		get_screen_resolution(screen_resolution.x, screen_resolution.y);

		// 3. reposition & resize
		// by default, the console will take half the resolution of the screen
		if (console_resolution.x == -1)
		{
			resolution.x = screen_resolution.x / 2;
			resolution.y = screen_resolution.y / 2;
		}
		else
		{
			resolution = console_resolution;
		}
		
		position.x = (screen_resolution.x - resolution.x) / 2;
		position.y = (screen_resolution.y - resolution.y) / 2;

		MoveWindow(handle, position.x, position.y, resolution.x, resolution.y, TRUE);

		// 4. hide scroll bar
		ShowScrollBar(handle, SB_BOTH, false);

		// 5. change stdout translation mode to support unicode character
		int reval = _setmode(_fileno(stdout), _O_U16TEXT);

		// 4. get buffer of visible view
		output_handle = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

		// 5. get row & columns count
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

		columns_count = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		rows_count = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		// 7. create our internal buffer
		framebuffer_byte_size = sizeof(CHAR_INFO) * columns_count * rows_count;
		framebuffer = (CHAR_INFO*)malloc(framebuffer_byte_size);
	}

	void clear()
	{
		memset(framebuffer, 0, framebuffer_byte_size);
	}

	void render_buffer()
	{
		SMALL_RECT region = { 0, 0, (SHORT)columns_count, (SHORT)rows_count };
		const COORD buffer_coord{ 0, 0 };
		const COORD buffer_size{ (SHORT)columns_count, (SHORT)rows_count };

		WriteConsoleOutput(output_handle, framebuffer, buffer_size, buffer_coord, &region);
	}

	void draw(const Vec2& position, WCHAR value)
	{
		framebuffer[position.x + position.y * columns_count].Char.AsciiChar = value;
		framebuffer[position.x + position.y * columns_count].Char.UnicodeChar = value;
		framebuffer[position.x + position.y * columns_count].Attributes = 0x0A;
	}

	void draw(const Cell& pixel)
	{
		draw(pixel.position, pixel.value);
	}

	void draw(const DrawComponent& draw_component)
	{
		for (u32 i = 0; i < draw_component.cells.size(); ++i)
		{
			i32 x = draw_component.position.x + draw_component.cells[i].position.x;
			i32 y = draw_component.position.y + draw_component.cells[i].position.y;

			draw({x, y}, draw_component.cells[i].value);
		}
	}

	void draw(const Actor& actor)
	{
		for (u32 i = 0; i < actor.draw_component.cells.size(); ++i)
		{
			const DrawComponent& draw_component = actor.draw_component;

			i32 x = actor.position.x + draw_component.position.x + draw_component.cells[i].position.x;
			i32 y = actor.position.y + draw_component.position.y + draw_component.cells[i].position.y;

			draw({ x, y }, draw_component.cells[i].value);
		}
	}
};


//enum class InputKey
//{
//	KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
//	KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
//	VK_SHIFT,
//
//};


//struct InputSystem
//{
//
//	enum class KeyCode : u32
//	{
//		//ENUM_START = 0,
//
//		Mouse_LBUTTON = 0,
//		Mouse_RBUTTON,
//		Mouse_CANCEL,
//		Mouse_MBUTTON,
//		Mouse_XBUTTON1,
//		Mouse_XBUTTON2,
//		KB_BACK,
//		KB_TAB,
//		KB_CLEAR,
//		KB_RETURN,
//		KB_SHIFT,
//		KB_CONTROL,
//		KB_MENU,
//		KB_PAUSE,
//		KB_CAPITAL,
//		KB_ESCAPE,
//		KB_SPACE,
//		KB_PRIOR,
//		KB_NEXT,
//		KB_END,
//		KB_HOME,
//		KB_LEFT,
//		KB_UP,
//		KB_RIGHT,
//		KB_DOWN,
//		KB_SELECT,
//		KB_PRINT,
//		KB_EXECUTE,
//		KB_SNAPSHOT,
//		KB_INSERT,
//		KB_DELETE,
//		KB_HELP,
//		KB_0,
//		KB_1,
//		KB_2,
//		KB_3,
//		KB_4,
//		KB_5,
//		KB_6,
//		KB_7,
//		KB_8,
//		KB_9,
//		KB_A,
//		KB_B,
//		KB_C,
//		KB_D,
//		KB_E,
//		KB_F,
//		KB_G,
//		KB_H,
//		KB_I,
//		KB_J,
//		KB_K,
//		KB_L,
//		KB_M,
//		KB_N,
//		KB_O,
//		KB_P,
//		KB_Q,
//		KB_R,
//		KB_S,
//		KB_T,
//		KB_U,
//		KB_V,
//		KB_W,
//		KB_X,
//		KB_Y,
//		KB_Z,
//		KB_LWIN,
//		KB_RWIN,
//		KB_NUMPAD0,
//		KB_NUMPAD1,
//		KB_NUMPAD2,
//		KB_NUMPAD3,
//		KB_NUMPAD4,
//		KB_NUMPAD5,
//		KB_NUMPAD6,
//		KB_NUMPAD7,
//		KB_NUMPAD8,
//		KB_NUMPAD9,
//		KB_MULTIPLY,
//		KB_ADD,
//		KB_SEPARATOR,
//		KB_SUBTRACT,
//		KB_DECIMAL,
//		KB_DIVIDE,
//		KB_F1,
//		KB_F2,
//		KB_F3,
//		KB_F4,
//		KB_F5,
//		KB_F6,
//		KB_F7,
//		KB_F8,
//		KB_F9,
//		KB_F10,
//		KB_F11,
//		KB_F12,
//		KB_LSHIFT,
//		KB_RSHIFT,
//		KB_LCONTROL,
//		KB_RCONTROL,
//		KB_LMENU,
//		KB_RMENU,
//
//		ENUM_END
//	};
//
//	static constexpr u32 VK_KEYCODES[] =
//	{
//		VK_LBUTTON,		// Left mouse button
//		VK_RBUTTON,		// Right mouse button
//		VK_CANCEL,		// Control - break processing
//		VK_MBUTTON,		// Middle mouse button(three - button mouse)
//		VK_XBUTTON1,	// X1 mouse button
//		VK_XBUTTON2,	// X2 mouse button
//		VK_BACK,		// BACKSPACE key
//		VK_TAB,			// TAB key
//		VK_CLEAR,		// CLEAR key
//		VK_RETURN,		// ENTER key
//		VK_SHIFT,		// SHIFT key
//		VK_CONTROL,		// CTRL key
//		VK_MENU,		// ALT key
//		VK_PAUSE,		// PAUSE key
//		VK_CAPITAL,		// CAPS LOCK key
//		VK_ESCAPE,		// ESC key
//		VK_SPACE,		// SPACEBAR
//		VK_PRIOR,		// PAGE UP key
//		VK_NEXT,		// PAGE DOWN key
//		VK_END,			// END key
//		VK_HOME,		// HOME key
//		VK_LEFT,		// LEFT ARROW key
//		VK_UP,			// UP ARROW key
//		VK_RIGHT,		// RIGHT ARROW key
//		VK_DOWN,		// DOWN ARROW key
//		VK_SELECT,		// SELECT key
//		VK_PRINT,		// PRINT key
//		VK_EXECUTE,		// EXECUTE key
//		VK_SNAPSHOT,	// PRINT SCREEN key
//		VK_INSERT,		// INS key
//		VK_DELETE,		// DEL key
//		VK_HELP,		// HELP key
//		0x30,			// 0
//		0x31,			// 1
//		0x32,			// 2
//		0x33,			// 3
//		0x34,			// 4
//		0x35,			// 5
//		0x36,			// 6
//		0x37,			// 7
//		0x38,			// 8
//		0x39,			// 9
//		0x41,			// A
//		0x42,			// B
//		0x43,			// C
//		0x44,			// D
//		0x45,			// E
//		0x46,			// F
//		0x47,			// G
//		0x48,			// H
//		0x49,			// I
//		0x4A,			// J
//		0x4B,			// K
//		0x4C,			// L
//		0x4D,			// M
//		0x4E,			// N
//		0x4F,			// O
//		0x50,			// P
//		0x51,			// Q
//		0x52,			// R
//		0x53,			// S
//		0x54,			// T
//		0x55,			// U
//		0x56,			// V
//		0x57,			// W
//		0x58,			// X
//		0x59,			// Y
//		0x5A,			// Z
//		VK_LWIN,		// Left Windows key(Natural keyboard)
//		VK_RWIN,		// Right Windows key(Natural keyboard)
//		VK_NUMPAD0,		// Numeric keypad 0 key
//		VK_NUMPAD1,		// Numeric keypad 1 key
//		VK_NUMPAD2,		// Numeric keypad 2 key
//		VK_NUMPAD3,		// Numeric keypad 3 key
//		VK_NUMPAD4,		// Numeric keypad 4 key
//		VK_NUMPAD5,		// Numeric keypad 5 key
//		VK_NUMPAD6,		// Numeric keypad 6 key
//		VK_NUMPAD7,		// Numeric keypad 7 key
//		VK_NUMPAD8,		// Numeric keypad 8 key
//		VK_NUMPAD9,		// Numeric keypad 9 key
//		VK_MULTIPLY,	// Multiply key
//		VK_ADD,			// Add key
//		VK_SEPARATOR,	// Separator key
//		VK_SUBTRACT,	// Subtract key
//		VK_DECIMAL,		// Decimal key
//		VK_DIVIDE,		// Divide key
//		VK_F1,			// F1 key
//		VK_F2,			// F2 key
//		VK_F3,			// F3 key
//		VK_F4,			// F4 key
//		VK_F5,			// F5 key
//		VK_F6,			// F6 key
//		VK_F7,			// F7 key
//		VK_F8,			// F8 key
//		VK_F9,			// F9 key
//		VK_F10,			// F10 key
//		VK_F11,			// F11 key
//		VK_F12,			// F12 key
//		VK_LSHIFT,		// Left SHIFT key
//		VK_RSHIFT,		// Right SHIFT key
//		VK_LCONTROL,	// Left CONTROL key
//		VK_RCONTROL,	// Right CONTROL key
//		VK_LMENU,		// Left MENU key
//		VK_RMENU		// Right MENU key
//	};
//
//	struct KeyInfo
//	{
//		KeyInfo(KeyCode code, u16 vk_code)
//			: keycode(code)
//			, vk_keycode(vk_code)
//			, is_pressed(false)
//			, was_pressed(false) 
//		{}
//
//		KeyInfo()
//			: keycode(KeyCode::ENUM_END)
//			, vk_keycode(0)
//			, is_pressed(false)
//			, was_pressed(false)
//		{}
//
//		KeyCode	keycode;
//		u16		vk_keycode;
//		bool	is_pressed;
//		bool	was_pressed;
//	};
//
//	std::unordered_map <KeyCode, KeyInfo> keys_current;
//
//	void init()
//	{
//		for (u32 i = u32(KeyCode::Mouse_LBUTTON); i < u32(KeyCode::ENUM_END); ++i)
//		{
//			keys_current[(KeyCode)i] = KeyInfo((KeyCode)i, VK_KEYCODES[i]);
//		}
//	}
//
//
//	void update()
//	{
//		for (auto it = keys_current.begin(); it != keys_current.end(); ++it)
//		{
//			KeyInfo& info = it->second;
//			info.was_pressed = info.is_pressed;
//			info.is_pressed = GetAsyncKeyState(info.vk_keycode) & 0x8000;
//		}
//	}
//
//	bool pressed(KeyCode code)
//	{
//		return keys_current[code].is_pressed;
//	}
//	bool released(KeyCode code)
//	{
//		return !(keys_current[code].is_pressed);
//	}
//
//	bool just_pressed(KeyCode code)
//	{
//		return keys_current[code].is_pressed && !(keys_current[code].was_pressed);
//	}
//
//	bool just_released(KeyCode code)
//	{
//		return !(keys_current[code].is_pressed) && keys_current[code].was_pressed;
//	}
//
//
//};

int main()
{
	ConsoleRenderer console_renderer;
	console_renderer.init({1920, 1080});

	snow::InputSystem input;
	input.init();

	Actor ship
	{ 
		// position
		{ 30, 31 }, 
		//draw component
		{
			// position
			{0, 0}, 
			// pixels
			{ 
				{ {0, 0}, '<'},
				{ {1, 0}, 'o'},
				{ {2, 0}, '|'}
			}
		}
	};



	DrawComponent title{ {50, 5} };
	DrawComponent::from_text("Chess 2.0\nYoooooo", title);

	while (true)
	{
		Sleep(16);

		input.update();

		console_renderer.clear();

		ship.position.x = (ship.position.x + 1) % console_renderer.columns_count;


		if (input.pressed(snow::InputSystem::KeyCode::KB_A))
		{
			OutputDebugString("A pressed\n");
		}

		if (input.just_pressed(snow::InputSystem::KeyCode::KB_B))
		{
			OutputDebugString("B just pressed\n");
		}

		if (input.just_released(snow::InputSystem::KeyCode::KB_C))
		{
			OutputDebugString("C just released\n");
		}

		console_renderer.draw(ship);

		console_renderer.render_buffer();
	}
}	