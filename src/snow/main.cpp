#include "stdio.h"


#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "types.h"

void get_desktop_resolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


int main()
{
	// 1. get console window
	HWND console_handle = GetConsoleWindow();

	// 2. get screen info
	int horizontal = 0;
	int vertical = 0;
	get_desktop_resolution(horizontal, vertical);
	
	// 3. reposition & resize
	int console_width = horizontal / 2;
	int console_height = vertical / 2;

	int top_left_x = (horizontal - console_width) / 2;
	int top_left_y = (vertical - console_height) / 2;

	MoveWindow(console_handle, top_left_x, top_left_y, console_width, console_height, TRUE); // 1920 width, 1080 height

	// 4. hide scroll bar
	ShowScrollBar(console_handle, SB_BOTH, false);

	// 5. change stdout translation mode to support unicode character
	int reval = _setmode(_fileno(stdout), _O_U16TEXT);

	// 4. get buffer of visible view
	HANDLE output_handle = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

	// 5. get row & columns count
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	SMALL_RECT region = { 0, 0, (SHORT)columns, (SHORT)rows };

	// 6. set terminal buffer to fit the window :: needed ?
	//SetConsoleWindowInfo(hOutput, TRUE, &rcRegion);
	//SetConsoleScreenBufferSize(hOutput, dwBufferSize);

	// 7. create our internal buffer
	CHAR_INFO* framebuffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * columns * rows);

	
	int x = 0;
	int y = 0;
	int z = 0;
	// 8. render
	while (true)
	{
		// clear
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				framebuffer[i + j * columns].Char.AsciiChar = '\0' + z;
				framebuffer[i + j * columns].Char.UnicodeChar = '\0' + z;
				framebuffer[i + j * columns].Attributes = 0;
			}
		}

		z++;

		Sleep(16);

		x = (x + 1) % columns;
		y = (y + 1) % rows;

		framebuffer[x + y * columns].Char.AsciiChar = '<';
		framebuffer[x + y * columns].Char.UnicodeChar = '<';
		framebuffer[x + y * columns].Attributes = 0x0A;

		framebuffer[x + 1 + y * columns].Char.AsciiChar = 'o';
		framebuffer[x + 1 + y * columns].Char.UnicodeChar = 'o';
		framebuffer[x + 1 + y * columns].Attributes = 0x0A;

		framebuffer[x + 2 + y * columns].Char.AsciiChar = '|';
		framebuffer[x + 2 + y * columns].Char.UnicodeChar = '|';
		framebuffer[x + 2 + y * columns].Attributes = 0x0A;

		COORD buffer_size{ columns, rows };
		WriteConsoleOutput(output_handle, framebuffer, buffer_size,
			{ 0, 0 }, &region);
	}
}