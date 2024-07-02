#include "access_client.h"

access_client::access_client(SOCKET s) : client(s), status(true) {}

access_client::access_client(const access_client& other): client(other.client), status(other.status.load()) {}

access_client& access_client::operator=(access_client const& other)
{
	if (this != &other)
	{
		client = other.client;
		status.store(other.status.load());
	}

	return *this;
}

access_client& access_client::operator=(access_client&& other) noexcept
{
	if (this != &other)
	{
		client = std::move(other.client);
		status.store(other.status.load());
	}

	other.client = INVALID_SOCKET;
	other.status.store(false);

	return *this;
}

access_client::access_client(access_client&& other) noexcept : client(std::move(other.client)), status(other.status.load())
{
	other.client = INVALID_SOCKET;
	other.status.store(false);
}

