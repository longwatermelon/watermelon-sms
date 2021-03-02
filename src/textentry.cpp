#include "../include/textentry.h"
#include "../include/graphics.h"


void TextEntry::render(GraphicsHandler* gfx)
{
	SDL_SetRenderDrawColor(gfx->rend, 0, 0, 255, 255);

	SDL_RenderFillRect(gfx->rend, &rect);
	SDL_RenderPresent(gfx->rend);

	SDL_SetRenderDrawColor(gfx->rend, 0, 0, 0, 255);
}


bool TextEntry::check_clicked(int cx, int cy)
{
	return ((cx > rect.x && cx < rect.x + rect.w) && (cy > rect.y && cy < rect.y + rect.h));
}


void TextEntry::recv_char(GraphicsHandler* gfx, char c)
{
	contents += c;
	draw_char(gfx, c);
}


void TextEntry::draw_char(GraphicsHandler* gfx, char c)
{
	char arr[] = { contents[contents.size() - 1], 0 };
	surf = TTF_RenderText_Solid(sans, arr, { 255, 255, 255 });
	msg_tex = SDL_CreateTextureFromSurface(gfx->rend, surf);

	SDL_Rect rtemp = { rect.x + contents.size() * CHAR_WIDTH, rect.y, CHAR_WIDTH, CHAR_HEIGHT };
	SDL_RenderCopy(gfx->rend, msg_tex, 0, &rtemp);
	SDL_RenderPresent(gfx->rend);
}


void TextEntry::clear_string(GraphicsHandler* gfx)
{
	contents = "";
	surf = TTF_RenderText_Solid(sans, contents.c_str(), { 255, 255, 255 });
	msg_tex = SDL_CreateTextureFromSurface(gfx->rend, surf);

	SDL_Rect rtemp = { rect.x, rect.y, CHAR_WIDTH, CHAR_HEIGHT };

	SDL_RenderClear(gfx->rend);
	SDL_RenderCopy(gfx->rend, msg_tex, 0, &rtemp);

	gfx->render_everything();
}