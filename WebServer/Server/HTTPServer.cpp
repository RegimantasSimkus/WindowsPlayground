#include <sstream>
#include <fstream>
#include "HTTPServer.h"

using namespace wpg::http;

bool HTTPServer::Startup()
{
	int err = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (err != 0)
	{
		printf("Failed to start webserver with error %d\n", err);
		return false;
	}

	if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
	{
		WSACleanup();
		printf("Couldnt find usable version of winsock.dll\n");
		return false;
	}

	return true;
}

bool HTTPServer::CreateServer()
{
	this->server = socket(AF_INET, SOCK_STREAM, 0);
	serverAddy.sin_port = htons(port);
	serverAddy.sin_family = AF_INET;
	serverAddy.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(server, (const sockaddr*)&serverAddy, sizeof(serverAddy));
	listen(server, 32);

	return true;
}

ClientSession HTTPServer::Listen() const
{
	sockaddr_in clientAddy{};
	int clientLen = sizeof(clientAddy);
	SOCKET client = accept(server, (sockaddr*)&clientAddy, &clientLen);
	return client;
}

bool HTTPServer::Close()
{
	if (server)
		closesocket(server);
	server = NULL;
	WSACleanup();
	return true;
}