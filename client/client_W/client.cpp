#include "client.h"
#pragma once

namespace Net
{
	Net::Server::Server(std::string localhost, std::string port) : massage(new char[maxbufer])
	{
		this->connects = INVALID_SOCKET;

		WORD DLL = MAKEWORD(2, 2);
		if (WSAStartup(DLL,&wsa) != 0)
		{
			printf("library didn't find!");
			exit(1);
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		if (bite = getaddrinfo(localhost.c_str(), port.c_str(), &this->hints, &this->object) != 0)
		{
			printf("getaddrinfo error!\n");
			shutdown_error();
		}

		connects = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);

		assert(connects != INVALID_SOCKET);


		if (bite = connect(connects, this->object->ai_addr, static_cast<int>(this->object->ai_addrlen)) == INVALID_SOCKET)
		{
			printf("error connection!\n");
			shutdown_error();
		}
		else
		{
			std::unique_lock<std::shared_mutex> lock(mutex);

			server_status.store(true);

			printf("Send your nickname: ");
			std::cin.getline(massage.get(), maxbufer);
			printf("\n");

			this->loged.emplace_back("client nickname : " + std::string(massage.get()));

			bite = send(connects, massage.get(), maxbufer, 0);

			if (bite == SOCKET_ERROR)
			{
				printf("error send!");
				shutdown_error();
			}
			else
			{
				std::thread thread_for_other_client(&Net::Server::client_other_info,this);
				thread_for_other_client.detach();
			}
		}
	}

	void Server::client_other_info()
	{
		while (server_status.load() == true)
		{
			bite = recv(this->connects, massage.get(), maxbufer, 0);

			if (bite < 0)
			{
				printf("error server shutdown\n");
				server_status.store(false);
				break;
			}
			else if (bite == 0)
			{
				printf("server shutdown");
				server_status.store(false);
				break;
			}
			else
			{
				std::unique_lock<std::shared_mutex> lock(mutex);

				massage.get()[bite] = '\0';

				this->loged.emplace_back(massage.get());

				printf(massage.get(),"\n");
				printf("\n");
			}
		}
	}

	void Server::client_chat()
	{
		while (server_status.load() == true)
		{
			std::cin.getline(massage.get(), maxbufer);
			printf("\n");

			if (strcmp(massage.get(),"exit") == 0 || strcmp(massage.get(), "Exit") == 0|| strcmp(massage.get(), "EXIT") == 0)
			{
				break;
			}
			else if(strlen(massage.get()) <= 0)
			{
				printf("massage is clear!");
				continue;
			}
			else
			{
				std::unique_lock<std::shared_mutex> lock(mutex);

				this->loged.emplace_back("My massage: " + std::string(massage.get()));

				bite = send(connects, massage.get(), maxbufer, 0);

				if (bite == SOCKET_ERROR)
				{
					printf("socket error to send massage!");
					shutdown_error();
				}
				else
				{
					continue;
				}
			}
		}
	}

	void Server::shutdown_error()
	{
		closesocket(this->connects);
		freeaddrinfo(this->object);
		WSACleanup();
		exit(1);
	}

	void Server::log()
	{
		std::fstream path("log.txt", std::fstream::out | std::fstream::app);

		if (path.is_open())
		{
			for (auto& i : loged)
			{
				path << i << std::endl;
			}
			path.close();
		}
		else
		{
			printf("File didn't create or can't find");
			path.close();
		}
	}
}
