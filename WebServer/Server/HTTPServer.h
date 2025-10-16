#pragma once
#include "../Core/HTTP.h"
#include "../Core/HTTPClient.h"
#include <string>

namespace wpg
{
	namespace http
	{
		class HTTPServer
		{
		public:
			HTTPServer(u_short port = 8881) : port(port), wsa{}, server{}
			{
			}

			bool Startup();

			bool CreateServer();

			ClientSession Listen() const;

			bool Close();

			u_short port;
			WSADATA wsa;
			SOCKET server;
			sockaddr_in serverAddy{};
		};
	}
}