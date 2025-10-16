#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include "Core/HTTP.h"
#include "Server/HTTPServer.h"
#include "Server/HTTPRouter.h"

namespace wpg
{
	namespace http
	{
		class WebServer : public HTTPServer
		{
		public:
			using HTTPServer::HTTPServer;

			bool Launch();

			bool Run() const;

			bool HandleRequest(ClientSession* session, std::string& request) const;

			HTTPRouter router;
		};
	}
}