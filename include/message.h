#pragma once
#include "constants.h"
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>


class Message
{
public:
	Message(const SDL_Rect& r, const std::string& msg)
		: rect(r), contents(msg), font(font)
	{
		font = TTF_OpenFont("OpenSans-Italic.ttf", 24);
	}

	void render(SDL_Renderer* rend, const SDL_Color& col = { 255, 255, 255 });

	void move(int x, int y);

	bool can_scroll() { return rect.y >= SCROLLING_Y; }

	std::string get_contents() { return contents; }

private:
	std::string contents;
	SDL_Rect rect;
	TTF_Font* font;
};