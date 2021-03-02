#include "../include/text.h"
#include "../include/graphics.h"


void Text::render(GraphicsHandler* gfx)
{
	SDL_Surface* stemp = TTF_RenderText_Solid(sans, contents.c_str(), { 255, 255, 255 });
	SDL_Texture* ttemp = SDL_CreateTextureFromSurface(gfx->rend, stemp);

	SDL_Rect rtemp = { rect.x + CHAR_WIDTH, rect.y, contents.size() * CHAR_WIDTH, CHAR_HEIGHT };
	SDL_RenderCopy(gfx->rend, ttemp, 0, &rtemp);

	SDL_FreeSurface(stemp);
	SDL_DestroyTexture(ttemp);
}