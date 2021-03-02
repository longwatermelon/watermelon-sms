#pragma once
#include "constants.h"
#include "text.h"
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>


class Message
{
public:
	Message(const SDL_Rect& r, const Text& t)
		: rect(r), text(t), font(font)
	{
		font = OPEN_FONT_SANS;
	}

	void render(SDL_Renderer* rend, const SDL_Color& col = { 255, 255, 255 });

	void move(int x, int y);

	bool can_scroll() { return rect.y >= SCROLLING_Y; }

	std::string get_contents() { return text.get_contents(); }

private:
	Text text;
	SDL_Rect rect;
	TTF_Font* font;
};