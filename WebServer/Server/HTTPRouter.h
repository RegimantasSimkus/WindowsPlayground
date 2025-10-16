#pragma once
#include "../Core/HTTP.h"
#include "../Core/HTTPClient.h"
#include <functional>
#include <vector>

namespace wpg
{
	namespace http
	{
		class HTTPRouter
		{
		public:
			using RouteCallback = std::function<bool(ClientSession* session, HTTPRequest& msg)>;

			struct RouteEntry_t
			{
				RouteEntry_t(HTTP_METHOD method, std::string url, RouteCallback callback) :
					method(method),
					url(url),
					callback(callback)
				{
				}

				HTTP_METHOD method;
				std::string url;
				RouteCallback callback;
			};

			HTTPRouter()
			{
			}

			static void AddRoute(HTTP_METHOD method, std::string url, RouteCallback callback);

			bool Handle(HTTP_METHOD method, HTTPRequest& msg, ClientSession* session) const;

			static std::vector<RouteEntry_t> Routes;
		};

		class HTTPRoute
		{
		public:
			HTTPRoute(HTTP_METHOD method, std::string url, HTTPRouter::RouteCallback callback)
			{
				HTTPRouter::AddRoute(method, url, callback);
			}
		};
	}
}