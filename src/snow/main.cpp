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
#include "console_renderer.h"
#include "actor.h"


int main()
{
	snow::ConsoleRenderer console_renderer;
	console_renderer.init({1920, 1080});

	snow::InputSystem input;
	input.init();


	snow::Actor ship
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

	snow::DrawComponent title{ {50, 5} };
	snow::DrawComponent::from_text("Chess 2.0\nYoooooo", title);

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

		console_renderer.draw(&ship);

		console_renderer.render_buffer();
	}
}	