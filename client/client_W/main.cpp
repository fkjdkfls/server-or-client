#include "client.h"

int main()
{
	Net::Server start("localhost", "1111");
	
	start.client_chat();
	
	return 0;
}