#pragma once
#include "constants.h"
#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>

class GraphicsHandler;


class Button
{
public:
	Button(const SDL_Rect& r, std::string text, const SDL_Color& col = { 100, 100, 100 })
		: rect(r), text(text) {}

	void render(GraphicsHandler* gfx);

	bool check_clicked(int cx, int cy);

	void bind_function(std::function<void()>* func);

	void call_function();

private:
	SDL_Rect rect;
	TTF_Font* font = OPEN_FONT_SANS;
	std::string text;

	std::function<void()> func;

	SDL_Color col;
};