#pragma once
#include"library.h"
#include"server_function.h"
#include"client_info.h"
#include"access_client.h"

namespace Net
{
	class server_function
	{
	public:
		std::atomic<bool> server_status;

		server_function(std::string namehost, std::string port);

		void log();

		void stop();

		void assert_info();

	private:
		std::shared_mutex mt;
		std::shared_ptr<char> massage;
		SOCKET sListen;
		ADDRINFOA hins;
		ADDRINFOA* object = nullptr;
		const int MAX_BUFFER = 1024;
		int bite{ 0 };
		std::vector<std::string> object_for_logs;
		std::vector<client_info> client_inform;
		std::vector<access_client> accepted_client;
		std::size_t counter;

		void client_info(int index);
	};

}
