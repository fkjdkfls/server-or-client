#include "client_info.h"

client_info::client_info() : client(0), nickname("") {}

client_info::client_info(SOCKET s, std::string nick): client(s), nickname(nick) {}

client_info::client_info(const client_info& other) : client(other.client), nickname(other.nickname) {}

client_info::client_info(client_info&& other) noexcept : client(std::move(other.client)), nickname(std::move(other.nickname))
{
	other.client = INVALID_SOCKET;
	other.nickname.clear();
}

client_info& client_info::operator=(client_info const& other)
{
	if (this != &other)
	{
		if (client != INVALID_SOCKET)
		{
			closesocket(client);
		}

		client = other.client;
		nickname = other.nickname;
	}

	return *this;
}

client_info& client_info::operator=(client_info&& other) noexcept
{
	if (this != &other)
	{
		if (client != INVALID_SOCKET)
		{
			closesocket(client);
		}

		client = std::move(other.client);
		nickname = std::move(other.nickname);
	}

	other.client = INVALID_SOCKET;
	other.nickname.clear();

	return *this;
}