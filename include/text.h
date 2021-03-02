#pragma once
#include "constants.h"
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class GraphicsHandler;


class Text
{
public:
	Text() = default;

	Text(const SDL_Rect& r, const std::string& text)
		: rect(r), contents(text)
	{
		sans = OPEN_FONT_SANS;
	}

	void render(GraphicsHandler* gfx);

	std::string get_contents() { return contents; }
	void set_contents(const std::string& t) { contents = t; }

	SDL_Rect get_rect() { return rect; }
	void move(int x, int y) { rect.x += x; rect.y += y; }

	TTF_Font* get_font() { return sans; }

private:
	SDL_Rect rect;
	std::string contents;
	
	TTF_Font* sans;
};