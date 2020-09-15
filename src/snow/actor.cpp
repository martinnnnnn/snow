#include "actor.h"


namespace snow
{
	void DrawComponent::from_text(const std::string& text, DrawComponent& output)
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

	void KeybordInputComponent::handle_input(const WCHAR value)
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

	void KeybordInputComponent::erase_last()
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
}