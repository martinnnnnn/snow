#include "console_renderer.h"

#include "actor.h"

namespace snow
{
	void get_screen_resolution(int& horizontal, int& vertical)
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		horizontal = desktop.right;
		vertical = desktop.bottom;
	}

	void ConsoleRenderer::init(const Vec2& console_resolution)
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

		void ConsoleRenderer::clear()
		{
			memset(framebuffer, 0, framebuffer_byte_size);
		}

		void ConsoleRenderer::render_buffer()
		{
			SMALL_RECT region = { 0, 0, (SHORT)columns_count, (SHORT)rows_count };
			const COORD buffer_coord{ 0, 0 };
			const COORD buffer_size{ (SHORT)columns_count, (SHORT)rows_count };

			WriteConsoleOutput(output_handle, framebuffer, buffer_size, buffer_coord, &region);
		}

		void ConsoleRenderer::draw(const Vec2& position, WCHAR value)
		{
			framebuffer[position.x + position.y * columns_count].Char.AsciiChar = value;
			framebuffer[position.x + position.y * columns_count].Char.UnicodeChar = value;
			framebuffer[position.x + position.y * columns_count].Attributes = 0x0A;
		}

		void ConsoleRenderer::draw(const Cell& pixel)
		{
			draw(pixel.position, pixel.value);
		}

		void ConsoleRenderer::draw(DrawComponent const * draw_component)
		{
			for (u32 i = 0; i < draw_component->cells.size(); ++i)
			{
				i32 x = draw_component->position.x + draw_component->cells[i].position.x;
				i32 y = draw_component->position.y + draw_component->cells[i].position.y;

				draw({x, y}, draw_component->cells[i].value);
			}
		}

		void ConsoleRenderer::draw(Actor const * actor)
		{
			for (u32 i = 0; i < actor->draw_component.cells.size(); ++i)
			{
				const DrawComponent& draw_component = actor->draw_component;

				i32 x = actor->position.x + draw_component.position.x + draw_component.cells[i].position.x;
				i32 y = actor->position.y + draw_component.position.y + draw_component.cells[i].position.y;

				draw({ x, y }, draw_component.cells[i].value);
			}
		}
}


