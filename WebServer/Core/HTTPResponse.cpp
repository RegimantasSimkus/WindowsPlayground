#include "HTTPResponse.h"
#include "HTTPFileTypes.h"

using namespace wpg::http;

// Server to Client response
std::string HTTPResponse::BuildResponse()
{
	std::string response =
		"HTTP/1.1 " + std::to_string(responseCode) + " " + message + "\r\n";

	for (auto& header : headers)
	{
		response.append(header.first + ": " + header.second + "\r\n");
	}

	response.append("\r\n" + content);
	return response;
}

void HTTPResponse::SetHeader(std::string name, std::string value)
{
	headers[name] = value;
}

void HTTPResponse::SetContent(std::string newContent, std::string contentType)
{
	content = newContent;
	headers["Content-Length"] = std::to_string(content.size());
	headers["Content-Type"] = contentType;
}

void HTTPResponse::SetContent(std::string newContent, HTTP_FILE contentType)
{
	SetContent(newContent, HTTPFileToMIME(contentType));
}