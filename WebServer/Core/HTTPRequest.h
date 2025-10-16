#pragma once
#include <string>
#include <map>

namespace wpg
{
	namespace http
	{
		// Client to Server request
		// https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Messages
		class HTTPRequest
		{

		private:
			char* _get_line(int index, int* lenOut = nullptr);
			char* _startline_segment(int index, int* lenOut = nullptr);

			std::string message;
		public:
			HTTPRequest(const std::string& msg) :
				message(msg),
				method(nullptr),
				url(nullptr),
				body(nullptr),
				version(nullptr)
			{
			}

			// Parse the message
			bool Parse();

			// GET/POST/...
			char* method;

			// such as /index.html?param=1
			char* url;

			// e.g HTTP/1.1
			char* version;

			// the body of the message
			char* body;

			// headers as key-values
			std::map<std::string, std::string> headers;
		protected:
		};
	}
}