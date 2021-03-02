#include "../include/button.h"
#include "../include/graphics.h"


void Button::render(GraphicsHandler* gfx)
{
	SDL_SetRenderDrawColor(gfx->rend, col.r, col.g, col.b, 255);
	SDL_RenderFillRect(gfx->rend, &rect);

	SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255 });
	SDL_Texture* tex = SDL_CreateTextureFromSurface(gfx->rend, surf);

	SDL_Rect r = { rect.x, rect.y, text.size() * CHAR_WIDTH, CHAR_HEIGHT };
	SDL_RenderCopy(gfx->rend, tex, 0, &r);

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(tex);

	SDL_SetRenderDrawColor(gfx->rend, 0, 0, 0, 255);
}


bool Button::check_clicked(int cx, int cy)
{
	return ((cx > rect.x && cx < rect.x + rect.w) && (cy > rect.y && cy < rect.y + rect.h));
}


void Button::bind_function(std::function<void()>* func)
{
	this->func = *func;
}


void Button::call_function()
{
	this->func();
}