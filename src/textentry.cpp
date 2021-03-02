#include "../include/textentry.h"
#include "../include/graphics.h"


void TextEntry::render(GraphicsHandler* gfx)
{
	if (focused)
	{
		SDL_SetRenderDrawColor(gfx->rend, 180, 180, 180, 255);
	}
	else
		SDL_SetRenderDrawColor(gfx->rend, 80, 80, 80, 255);

	SDL_RenderFillRect(gfx->rend, &rect);

	if (focused) { SDL_SetRenderDrawColor(gfx->rend, 0, 200, 200, 255); SDL_RenderDrawRect(gfx->rend, &rect); }
	

	SDL_Surface* stemp = TTF_RenderText_Solid(t.get_font(), t.get_contents().c_str(), { 255, 255, 255 });
	SDL_Texture* ttemp = SDL_CreateTextureFromSurface(gfx->rend, stemp);

	SDL_Rect rtemp = { rect.x + CHAR_WIDTH, rect.y, t.get_contents().size() * CHAR_WIDTH, CHAR_HEIGHT };
	SDL_RenderCopy(gfx->rend, ttemp, 0, &rtemp);

	SDL_RenderPresent(gfx->rend);

	SDL_FreeSurface(stemp);
	SDL_DestroyTexture(ttemp);

	SDL_SetRenderDrawColor(gfx->rend, 0, 0, 0, 255);
}


bool TextEntry::check_clicked(int cx, int cy)
{
	bool cond = ((cx > rect.x && cx < rect.x + rect.w) && (cy > rect.y && cy < rect.y + rect.h));

	if (cond) focused = true;
	else focused = false;

	return cond;
}


void TextEntry::recv_char(GraphicsHandler* gfx, char c)
{
	t.set_contents(t.get_contents() + c);
	draw_char(gfx, c);
}


void TextEntry::draw_char(GraphicsHandler* gfx, char c)
{
	std::string contents = t.get_contents();
	TTF_Font* sans = t.get_font();

	char arr[] = { contents[contents.size() - 1], 0 };
	surf = TTF_RenderText_Solid(sans, arr, { 255, 255, 255 });
	msg_tex = SDL_CreateTextureFromSurface(gfx->rend, surf);

	SDL_Rect rtemp = { rect.x + contents.size() * CHAR_WIDTH, rect.y, CHAR_WIDTH, CHAR_HEIGHT };
	SDL_RenderCopy(gfx->rend, msg_tex, 0, &rtemp);
	SDL_RenderPresent(gfx->rend);
}


void TextEntry::clear_string(GraphicsHandler* gfx)
{
	t.set_contents("");
	surf = TTF_RenderText_Solid(t.get_font(), t.get_contents().c_str(), { 255, 255, 255 });
	msg_tex = SDL_CreateTextureFromSurface(gfx->rend, surf);

	SDL_Rect rtemp = { rect.x, rect.y, CHAR_WIDTH, CHAR_HEIGHT };

	SDL_RenderClear(gfx->rend);
	SDL_RenderCopy(gfx->rend, msg_tex, 0, &rtemp);

	gfx->render_everything();
}