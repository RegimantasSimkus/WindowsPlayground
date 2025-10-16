#pragma once
#include "HTTPFileTypes.h"
#include <string>
#include <map>

namespace wpg
{
	namespace http
	{
		// Server to Client response
		class HTTPResponse
		{
		public:
			HTTPResponse() : content(), responseCode(200), message("OK")
			{
			}

			HTTPResponse(int responseCode, std::string message) : responseCode(responseCode), message(message)
			{
			}

			std::string BuildResponse();

			void SetHeader(std::string name, std::string value);

			void SetContent(std::string newContent, std::string contentType);

			void SetContent(std::string newContent, HTTP_FILE contentType);

			std::map<std::string, std::string> headers;

			int responseCode;
			std::string message;
			std::string content;
		};
	}
}