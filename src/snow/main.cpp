#include "stdio.h"


#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "types.h"

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

	void init()
	{
		// 1. get console window
		handle = GetConsoleWindow();

		// 2. get screen info
		get_screen_resolution(screen_resolution.x, screen_resolution.y);

		// 3. reposition & resize
		resolution.x = screen_resolution.x / 2;
		resolution.y = screen_resolution.y / 2;

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
		const COORD buffer_size{ columns_count, rows_count };

		WriteConsoleOutput(output_handle, framebuffer, buffer_size, buffer_coord, &region);
	}

	void draw(const Vec2& position, WCHAR value)
	{
		framebuffer[position.x + position.y * columns_count].Char.AsciiChar = value;
		framebuffer[position.x + position.y * columns_count].Char.UnicodeChar = value;
		framebuffer[position.x + position.y * columns_count].Attributes = 0x0A;
	}
};



int main()
{
	ConsoleRenderer console_renderer;

	console_renderer.init();

	Vec2 ship_position{30, 30};

	while (true)
	{
		Sleep(16);

		console_renderer.clear();

		ship_position.x = (ship_position.x + 1) % console_renderer.columns_count;

		console_renderer.draw(ship_position, '<');
		console_renderer.draw({ ship_position.x + 1, ship_position.y}, 'o');
		console_renderer.draw({ ship_position.x + 2, ship_position.y }, '|');

		console_renderer.render_buffer();
	}
}