#include "../include/graphics.h"


GraphicsHandler::GraphicsHandler(int w, int h)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	screen_w = w;
	screen_h = h;

	window = SDL_CreateWindow("watermelon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);

	render_everything();
}


void GraphicsHandler::mainloop()
{
	for (auto& m : messages)
	{
		if (m->can_scroll())
		{
			for (int i = 0; i < messages.size(); i++)
			{
				messages[i]->move(0, -CHAR_HEIGHT);
			}

			render_everything();
			break;
		}
	}

	int end = -1;
	for (int i = messages.size() - 1; i > 0; i--)
	{
		if (messages[i]->scheduled_destruction())
		{
			end = i;
			break;
		}
	}

	if (end != -1)
		messages.erase(messages.begin(), messages.begin() + end);
}


void GraphicsHandler::render_everything()
{
	SDL_RenderClear(rend);

	for (auto& m : messages)
		m->render(rend);

	for (auto& e : entries)
		e->render(this);

	for (auto& b : buttons)
		b->render(this);

	SDL_RenderPresent(rend);
}


void GraphicsHandler::cleanup()
{
	if (window) SDL_DestroyWindow(window);
	if (rend) SDL_DestroyRenderer(rend);
}