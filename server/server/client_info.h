#pragma once
#include"library.h"

class client_info
{
public:
	SOCKET client;
	std::string nickname;

	client_info();

	client_info(SOCKET s, std::string nick);

	client_info(const client_info& other);

	client_info(client_info&& other) noexcept;

	client_info& operator =(client_info const& other);

	client_info& operator =(client_info&& other) noexcept;
};

