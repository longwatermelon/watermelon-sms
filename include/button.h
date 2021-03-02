#pragma once
#include "constants.h"
#include "text.h"
#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>

class GraphicsHandler;


class Button
{
public:
	Button(const SDL_Rect& r, const Text& text, const SDL_Color& col = { 100, 100, 100 })
		: rect(r), t(text) {}

	void render(GraphicsHandler* gfx);

	bool check_clicked(int cx, int cy);

	void bind_function(std::function<void()>* func);

	void call_function();

private:
	SDL_Rect rect;
	Text t;

	std::function<void()> func;

	SDL_Color col;
};