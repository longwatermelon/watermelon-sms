#include "../include/constants.h"
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


void receive(tcp::socket* sock, std::mutex* mtx, GraphicsHandler* gfx)
{
	SDL_Rect mrect = { 0, 0, 0, CHAR_HEIGHT };

	while (true)
	{
		sock->wait(sock->wait_read);

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


void input(tcp::socket* sock)
{
	while (true)
	{
		std::string msg;
		std::getline(std::cin, msg);

		send(sock, msg);
	}
}


int main(int argc, char* argv[])
{
	GraphicsHandler gfx;

	asio::io_service service;
	tcp::socket sock(service);

	sock.connect(tcp::endpoint(address::from_string("127.0.0.1"), 1234));
	
	std::mutex mtx;
	std::thread thr_recv(receive, &sock, &mtx, &gfx);
	std::thread thr_inp(input, &sock);

	bool running = true;
	SDL_Event evt;

	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT: running = false; break;
			}
		}

		{
			std::lock_guard lock(mtx);
			gfx.mainloop();
		}
	}

	gfx.cleanup();

	SDL_Quit();
	TTF_Quit();

	return 0;
}