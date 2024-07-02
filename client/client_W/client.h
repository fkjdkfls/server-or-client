#include"library.h"

namespace Net
{
	class Server
	{
	public:
		std::atomic<bool> server_status;

		const int maxbufer = 512;

		Server(std::string localhost, std::string port);

	private:

		void shutdown_error();

		void log();

		void client_other_info();

	public:

		void client_chat();

	private:
		WSAData wsa;
		ADDRINFOA hints;
		ADDRINFOA* object = nullptr;
		SOCKET connects;
		std::shared_mutex mutex;
		std::shared_ptr<char[]> massage;
		int bite = 0;
		std::vector<std::string> loged;
	};

}