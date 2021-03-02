#include "../include/constants.h"
#include "../include/textentry.h"
#include "../include/graphics.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <SDL.h>
#include <SDL_ttf.h>

using namespace asio::ip;


namespace userinfo
{
	std::shared_ptr<TextEntry> selected_entry;
}


void receive(tcp::socket* sock, std::mutex* mtx, GraphicsHandler* gfx, bool* running)
{
	SDL_Rect mrect = { 0, 0, 0, CHAR_HEIGHT };

	while (*running)
	{
		if (sock->available() > 0)
		{
			std::vector<char> buf(sock->available());
			sock->read_some(asio::buffer(buf.data(), buf.size()));

			std::string data;
			for (char c : buf)
				data += c;

			mrect.w = data.size() * CHAR_WIDTH;

			{
				std::lock_guard lock(*mtx);

				gfx->messages.push_back(std::make_shared<Message>(mrect, data));

				gfx->messages[gfx->messages.size() - 1]->render(gfx->rend);

				gfx->render_everything();
			}

			if (mrect.y < SCROLLING_Y)
				mrect.y += CHAR_HEIGHT;
		}

		SDL_Delay(10);
	}
}


void send(tcp::socket* sock, const std::string& msg)
{
	if (msg.size() >= 100)
	{
		std::cout << "messages over 100 characters are invalid\n";
		return;
	}

	sock->write_some(asio::buffer(msg.data(), msg.size()));
}


void mousepress(GraphicsHandler& gfx, SDL_MouseButtonEvent& b)
{
	if (b.button == SDL_BUTTON_LEFT)
	{
		bool focused = false;

		for (auto& e : gfx.entries)
		{
			int cx, cy;
			SDL_GetMouseState(&cx, &cy);

			if (e->check_clicked(cx, cy))
			{
				userinfo::selected_entry = e;
				focused = true;
			}
		}

		if (!focused) userinfo::selected_entry = nullptr;

		gfx.render_everything();
	}
}


int main(int argc, char* argv[])
{
	GraphicsHandler gfx;

	asio::io_service service;
	tcp::socket sock(service);

	sock.connect(tcp::endpoint(address::from_string("127.0.0.1"), 1234));
	
	bool running = true;

	std::mutex mtx;
	std::thread thr_recv(receive, &sock, &mtx, &gfx, &running);


	gfx.entries.push_back(std::make_shared<TextEntry>(SDL_Rect{ 0, 480, 500, 20 }));
	for (auto& e : gfx.entries)
		e->render(&gfx);

	SDL_RenderPresent(gfx.rend);
	
	SDL_Event evt;

	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT: running = false; break;
			case SDL_MOUSEBUTTONDOWN: mousepress(gfx, evt.button); break;

			case SDL_TEXTINPUT: {
				if (userinfo::selected_entry)
				{
					userinfo::selected_entry->recv_char(&gfx, evt.text.text[0]);
				}
			} break;

			case SDL_KEYDOWN: {
				switch (evt.key.keysym.scancode)
				{
				case SDL_SCANCODE_RETURN: {
					if (userinfo::selected_entry)
					{
						std::lock_guard lock(mtx);
						send(&sock, userinfo::selected_entry->str());
						userinfo::selected_entry->clear_string(&gfx);
					}
				} break;
				case SDL_SCANCODE_ESCAPE: running = false; break;
				}
			} break;
			}
		}

		{
			std::lock_guard lock(mtx);
			gfx.mainloop();
		}
	}

	sock.close();
	gfx.cleanup();

	if (thr_recv.joinable())
	{
		thr_recv.join();
	}

	return 0;
}