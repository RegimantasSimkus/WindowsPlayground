#include "HTTPRouter.h"

using namespace wpg::http;

std::vector<HTTPRouter::RouteEntry_t> HTTPRouter::Routes{};

void HTTPRouter::AddRoute(HTTP_METHOD method, std::string url, RouteCallback callback)
{
	Routes.push_back({ method, url, callback });
}

bool HTTPRouter::Handle(HTTP_METHOD method, HTTPRequest& msg, ClientSession* session) const
{
	bool found = false;
	bool success = false;
	if (method == HTTP_NULL)
	{
		session->SendQuickResponse(400, "Bad method");
		return false;
	}

	for (auto& entry : Routes)
	{
		if (entry.method == method && entry.url == msg.url)
		{
			success = entry.callback(session, msg);
			found = true;
			break;
		}
	}

	if (!found)
	{
		if (method == HTTP_GET)
		{
			std::string filePath = msg.url;
			if (filePath.front() == '/')
				filePath.erase(filePath.begin());

			if (filePath.empty())
				filePath = "index.html";

			if (session->SendFile("html/" + filePath))
				return true;
			session->SendQuickResponse(403, "Forbidden");
		}
	}
	else if (!success)
	{
		session->SendQuickResponse(500, "Server error");
	}

	return found;
}