#include "HTTPFileTypes.h"

using namespace wpg::http;

const char* wpg::http::HTTPFileToMIME(HTTP_FILE fileEnum)
{
	int index = 0;
	for (int i = 0; i < (sizeof(HTTP_FILE_TYPES) / sizeof(*HTTP_FILE_TYPES)); i++)
	{
		const HTTP_FILE_TYPE* entry = &HTTP_FILE_TYPES[i];
		if (entry->type == fileEnum)
		{
			index = i;
			break;
		}
	}
	return HTTP_FILE_TYPES[index].mime;
}

HTTP_FILE wpg::http::AssumeFileType(const std::string& fileName)
{
	size_t offs = fileName.find_last_of('.');
	if (offs == std::string::npos)
		return HTTP_FILE_PLAIN;

	const char* extension = &fileName[offs];
	for (int i = 0; i < (sizeof(HTTP_FILE_TYPES) / sizeof(*HTTP_FILE_TYPES)); i++)
	{
		const HTTP_FILE_TYPE* entry = &HTTP_FILE_TYPES[i];
		if (!strcmp(extension, entry->extension))
			return entry->type;
	}
	return HTTP_FILE_PLAIN;
}