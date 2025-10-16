#include "WebServer.h"

using namespace wpg::http;

bool WebServer::Launch()
{
	if (!this->Startup())
		return false;

	printf("WSA Startup successful.\n");
	if (!this->CreateServer())
	{
		this->Close();
		return false;
	}

	printf("Server creation successful.\n");
	return true;
}

bool WebServer::Run() const
{
	ClientSession session = this->Listen();
	if (!session)
		return false;

	std::string request = session.Receive();
#ifdef _DEBUG
	printf("Received request:\n%s\n", request.c_str());
#endif
	if (request.empty())
		return false;

	HandleRequest(&session, request);
	return true;
}

bool WebServer::HandleRequest(ClientSession* session, std::string& request) const
{
	HTTPRequest msg = request;
	if (!msg.Parse())
		return false; // bad http message, disregard

	HTTP_METHOD method = GetHTTPMethod(msg.method);
	return router.Handle(method, msg, session);
}
