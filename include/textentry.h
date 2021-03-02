#pragma once
#include "constants.h"
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class GraphicsHandler;


class TextEntry
{
public:
	TextEntry(const SDL_Rect& r)
		: rect(r) {}

	~TextEntry()
	{
		if (surf) SDL_FreeSurface(surf);
		if (msg_tex) SDL_DestroyTexture(msg_tex);
	}

	void render(GraphicsHandler* gfx);

	bool check_clicked(int cx, int cy);

	void recv_char(GraphicsHandler* gfx, char c);
	void draw_char(GraphicsHandler* gfx, char c);

	std::string str() { return contents; }

	void clear_string(GraphicsHandler* gfx);

private:
	SDL_Rect rect;
	std::string contents;
	TTF_Font* sans = OPEN_FONT_SANS;

	SDL_Surface* surf{ nullptr };
	SDL_Texture* msg_tex{ nullptr };
};