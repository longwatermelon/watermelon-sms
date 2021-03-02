#include "../include/message.h"


void Message::render(SDL_Renderer* rend, const SDL_Color& col)
{
	SDL_Surface* surf = TTF_RenderText_Solid(font, contents.c_str(), col);
	SDL_Texture* msg = SDL_CreateTextureFromSurface(rend, surf);

	SDL_RenderCopy(rend, msg, 0, &rect);
	SDL_FreeSurface(surf);
	SDL_DestroyTexture(msg);
}


void Message::move(int x, int y)
{
	rect.x += x;
	rect.y += y;
}