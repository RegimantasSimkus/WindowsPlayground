#pragma once
#include <string>

namespace wpg
{
	namespace http
	{
		enum HTTP_FILE
		{
			HTTP_FILE_PLAIN = 0, // plain text
			HTTP_FILE_HTML, // html documents
			HTTP_FILE_JAVASCRIPT, // javascript files
			HTTP_FILE_JS = HTTP_FILE_JAVASCRIPT,
			HTTP_FILE_JSON, // javascript files
			HTTP_FILE_CSS, // css files
			HTTP_FILE_PNG, // png
			HTTP_FILE_JPEG, // png
			HTTP_FILE_MARKDOWN, // markdown files
		};

		// for internal file type map
		struct HTTP_FILE_TYPE
		{
			enum HTTP_FILE type;
			const char* mime;
			const char* extension;
		};

		static const HTTP_FILE_TYPE HTTP_FILE_TYPES[]{
			{HTTP_FILE_PLAIN, "text/plain", ""},
			{HTTP_FILE_HTML, "text/html", ".html"},
			{HTTP_FILE_JAVASCRIPT, "application/javascript", ".js"},
			{HTTP_FILE_JSON, "application/json", ".json"},
			{HTTP_FILE_CSS, "text/css", ".css"},
			{HTTP_FILE_PNG, "image/png", ".png"},
			{HTTP_FILE_JPEG, "image/jpeg", ".jpg"},
			{HTTP_FILE_MARKDOWN, "text/plain", ".md"},
		};

		const char* HTTPFileToMIME(HTTP_FILE fileEnum);

		HTTP_FILE AssumeFileType(const std::string& fileName);
	}
}