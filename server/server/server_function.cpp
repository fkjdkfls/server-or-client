#include"library.h"
#include"server_function.h"

namespace Net
{
	server_function::server_function(std::string namehost, std::string port) : counter(NULL)
	{
		massage.reset(new char[MAX_BUFFER], std::default_delete<char[]>());

		WSAData WSA;
		WORD dll = MAKEWORD(2, 2);
		assert(WSAStartup(dll, &WSA) == 0);

		ZeroMemory(&hins, sizeof(hins));

		hins.ai_family = AF_INET;
		hins.ai_flags = AI_PASSIVE;
		hins.ai_socktype = SOCK_STREAM;
		hins.ai_protocol = IPPROTO_TCP;

		bite = getaddrinfo(namehost.c_str(), port.c_str(), &hins, &object);
		assert(bite == 0);

		sListen = socket(object->ai_family, object->ai_socktype, object->ai_protocol);
		assert(sListen != INVALID_SOCKET);

		bite = bind(sListen, object->ai_addr, static_cast<int>(object->ai_addrlen));
		assert(bite != SOCKET_ERROR);

		bite = listen(sListen, SOMAXCONN);
		assert(bite != SOCKET_ERROR);
		
		server_status.store(true);
	}

	void server_function::log()
	{
		std::fstream path("log.txt", std::fstream::app | std::fstream::out);

		if (path.is_open())
		{
			for (auto& i : object_for_logs)
			{
				path << i << std::endl;
			}

			path.close();
		}
		else
		{
			path.close();
		}
	}

	void server_function::stop()
	{
		std::thread Thread_for_Exit_server([this]()
			{
				std::string exit_massage;

				while (server_status)
				{
					std::getline(std::cin, exit_massage);

					if (exit_massage == "exit" || exit_massage == "Exit"|| exit_massage == "EXIT")
					{
						break;
					}
					else
					{
						continue;
					}
				}

				server_status.store(false);

				for (auto& i : accepted_client)
				{
					closesocket(i.client);
					i.status.store(false);
				}
				closesocket(sListen);
				freeaddrinfo(object);
				WSACleanup();
			});
		Thread_for_Exit_server.detach();
	}

	void server_function::assert_info()
	{
		while (server_status)
		{
			SOCKET clients = accept(sListen, object->ai_addr, reinterpret_cast<int *>(&object->ai_addrlen));

			if (clients == INVALID_SOCKET)
			{
				closesocket(clients);
				break;
			}
			else
			{
				bite = recv(clients, massage.get(), MAX_BUFFER, 0);

				if (bite <= 0)
				{
					std::cout << "client error!" << std::endl;
					closesocket(clients);
					break;
				}
				else
				{
					std::cout << "client connected!" << std::endl;

					std::unique_lock<std::shared_mutex> lock(mt);

					massage.get()[bite] = '\0';

					client_inform.emplace_back(clients, massage.get());
					accepted_client.emplace_back(clients);
					object_for_logs.emplace_back(massage.get());

					++counter;
				}
				std::thread thread_for_other_info(&Net::server_function::client_info,this ,accepted_client.size() - 1);
				thread_for_other_info.detach();
			}
		}
	}

	void server_function::client_info(int index)
	{
		SOCKET clients = accepted_client[index].client;

		while (server_status)
		{
			if (clients == INVALID_SOCKET)
			{
				closesocket(clients);
				accepted_client[index].status.store(false);
				std::cout << "client socket_invalid" << std::endl;
				break;
			}
			else
			{
				bite = recv(clients, massage.get(), MAX_BUFFER, 0);

				if (bite <= 0)
				{
					std::cout << "client " << index << " disconect" << std::endl;
					closesocket(clients);
					accepted_client[index].status.store(false);
					break;
				}
				else
				{
					std::unique_lock<std::shared_mutex> lock(mt);

					massage.get()[bite] = '\0';

					std::string m = client_inform[index].nickname + ": " + std::string(massage.get(), bite);
					object_for_logs.emplace_back(m);

					for (std::size_t i = 0; i < counter; ++i)
					{
						if (i == index || !accepted_client[i].status)
						{
							continue;
						}
						send(accepted_client[i].client, m.c_str(), static_cast<int>(m.size()), 0);
					}
				}
			}
		}
	}
}

