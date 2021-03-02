#include <iostream>
#include <thread>
#include <memory>
#include <chrono>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>

using namespace asio::ip;

std::vector<std::shared_ptr<tcp::socket>> g_users;
std::mutex g_mutex;

void broadcast(const std::string& msg, int ignored);


void receive()
{
	while (true)
	{
		{
			std::lock_guard<std::mutex> lock(g_mutex);
			std::string data;

			for (int i = 0; i < g_users.size(); i++)
			{
				std::shared_ptr<tcp::socket>& sock = g_users[i];
				if (!sock->is_open())
				{
					continue;
				}

				size_t bytes = sock->available();

				if (bytes > 0)
				{
					std::cout << "read " << bytes << " bytes\n";

					std::vector<char> buf(bytes);

					sock->read_some(asio::buffer(buf.data(), buf.size()));

					std::string data;
					for (char c : buf)
						data += c;

					broadcast(data, i);
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // delay so other threads can access g_users
	}
}


void broadcast(const std::string& msg, int ignored)
{
	for (int i = 0; i < g_users.size(); i++)
	{
		if (i == ignored)
		{
			std::string m = "You: " + msg;
			g_users[i]->write_some(asio::buffer(m.data(), m.size()));
			continue;
		}

		g_users[i]->write_some(asio::buffer(msg.data(), msg.size()));
	}
}


void accept_users(tcp::acceptor* act, asio::io_service* service)
{
	while (true)
	{
		auto sock = std::make_shared<tcp::socket>(*service);
		act->accept(*sock);

		{
			std::lock_guard<std::mutex> lock(g_mutex);
			g_users.push_back(sock);
		}
	}
}


int main()
{
	asio::io_service service;
	tcp::acceptor act(service, tcp::endpoint(tcp::v4(), 1234));

	std::thread thr_accept(accept_users, &act, &service);
	std::thread thr_recv(receive);

	while (true);

	return 0;
}