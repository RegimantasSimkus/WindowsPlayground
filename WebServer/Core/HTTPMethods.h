#pragma once
#include <string>

namespace wpg
{
	namespace http
	{
		enum HTTP_METHOD
		{
			HTTP_NULL = 0,
			HTTP_GET,
			HTTP_POST,
			HTTP_PUT
		};

		struct HTTP_METHOD_TYPE
		{
			HTTP_METHOD type;
			const char* name;
		};

		static const HTTP_METHOD_TYPE HTTP_METHOD_TYPES[] = {
			{HTTP_GET, "GET"},
			{HTTP_POST, "POST"},
			{HTTP_PUT, "PUT"}
		};

		HTTP_METHOD GetHTTPMethod(const std::string& text);
	}
}