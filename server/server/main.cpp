#include"library.h"
#include"access_client.h"
#include"client_info.h"
#include"server_function.h"

int main()
{
	Net::server_function object("localhost", "1111");

	object.stop();

	while (object.server_status)
	{
		object.assert_info();
	}

	object.log();

	return 0;
}