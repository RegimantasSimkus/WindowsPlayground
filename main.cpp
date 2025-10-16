
#include <iostream>

#include "WindowsOS/RegistryProcedures.h"
#include "WebServer/WebServer.h"

int main()
{
	wpg::http::WebServer webserver(8888);
	if (!webserver.Launch())
		throw "Failed to launch webserver??";

	while (1)
	{
		webserver.Run();
	}
}