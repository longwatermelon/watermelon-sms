#include "../include/constants.h"
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


void receive(tcp::socket* sock, std::mutex* mtx)
{
	while (true)
	{
		sock->wait(sock->wait_read);

		std::vector<char> buf(sock->available());
		sock->read_some(asio::buffer(buf.data(), buf.size()));

		std::string data;
		for (char c : buf)
			data += c;

		std::cout << data;
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
	asio::io_service service;
	tcp::socket sock(service);

	sock.connect(tcp::endpoint(address::from_string("127.0.0.1"), 1234));
	
	std::mutex mtx;
	std::thread thr_recv(receive, &sock, &mtx);
	std::thread thr_inp(input, &sock);

	while (true);

	return 0;
}