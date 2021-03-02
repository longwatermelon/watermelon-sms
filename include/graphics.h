#pragma once
#include "message.h"
#include "textentry.h"
#include <vector>
#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>


class GraphicsHandler
{
public:
	SDL_Window* window;
	SDL_Renderer* rend;

	int screen_w;
	int screen_h;

	std::vector<std::shared_ptr<Message>> messages;
	std::vector<std::shared_ptr<TextEntry>> entries;

	GraphicsHandler(int w = 500, int h = 500);

	void mainloop();

	void render_everything();

	void cleanup();
};