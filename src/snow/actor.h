#pragma once

#include "stdio.h"


#include <vector>
#include <string>
#include <array>
#include <fmt/format.h>

#include "types.h"
#include "input.h"
#include "math.h"
#include "console_renderer.h"

namespace snow
{
	struct DrawComponent
	{
		Vec2 position;
		std::vector<Cell> cells;

		static void from_text(const std::string& text, DrawComponent& output);
	};

	struct KeybordInputComponent
	{
		Vec2 position;
		Vec2 current_position;
		std::vector<Cell> cells;

		void handle_input(const WCHAR value);
		void erase_last();
	};

	struct Actor
	{
		Vec2 position;
		DrawComponent draw_component;
	};
}