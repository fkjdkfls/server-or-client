#pragma once
#include"library.h"

class access_client
{
public:

	SOCKET client;
	std::atomic<bool> status;

	access_client();

	access_client(SOCKET s);

	access_client(const access_client& other);

	access_client(access_client&& other) noexcept;

	access_client& operator =(access_client const& other);

	access_client& operator =(access_client&& other) noexcept;
};

