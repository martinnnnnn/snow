#include "stdio.h"


#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <vector>

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

struct Pixel
{
	Vec2 position;
	WCHAR value;
};


struct DrawComponent
{
	Vec2 position;
	std::vector<Pixel> pixels;
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
		
;

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

	void draw(const Pixel& pixel)
	{
		draw(pixel.position, pixel.value);
	}

	void draw(const DrawComponent& draw_component)
	{
		for (u32 i = 0; i < draw_component.pixels.size(); ++i)
		{
			i32 x = draw_component.position.x + draw_component.pixels[i].position.x;
			i32 y = draw_component.position.y + draw_component.pixels[i].position.y;

			draw({x, y}, draw_component.pixels[i].value);
		}
	}

	void draw(const Actor& actor)
	{
		for (u32 i = 0; i < actor.draw_component.pixels.size(); ++i)
		{
			const DrawComponent& draw_component = actor.draw_component;

			i32 x = actor.position.x + draw_component.position.x + draw_component.pixels[i].position.x;
			i32 y = actor.position.y + draw_component.position.y + draw_component.pixels[i].position.y;

			draw({ x, y }, draw_component.pixels[i].value);
		}
	}
};


int main()
{
	ConsoleRenderer console_renderer;

	console_renderer.init({1920, 1080});

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

	while (true)
	{
		Sleep(16);

		console_renderer.clear();

		ship.position.x = (ship.position.x + 1) % console_renderer.columns_count;

		console_renderer.draw(ship);

		console_renderer.render_buffer();
	}
}