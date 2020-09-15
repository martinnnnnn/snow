#pragma once

#include <io.h>
#include <fcntl.h>
#include <windows.h>

#include "types.h"
#include "math.h"

namespace snow
{
	void get_screen_resolution(int& horizontal, int& vertical);

	struct DrawComponent;
	struct Actor;

	struct Cell
	{
		Cell() : position(), value() {}
		Cell(Vec2 pos, WCHAR val) : position(pos), value(val) {}

		Vec2 position;
		WCHAR value;
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

		void init(const Vec2& console_resolution = Vec2{ -1, -1 });
		void clear();
		void render_buffer();

		void draw(const Vec2& position, WCHAR value);
		void draw(const Cell& pixel);
		void draw(DrawComponent const * draw_component);
		void draw(Actor const* actor);
	};
}