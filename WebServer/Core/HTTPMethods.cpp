#include "HTTPMethods.h"

using namespace wpg::http;

HTTP_METHOD wpg::http::GetHTTPMethod(const std::string& text)
{
	for (int i = 0; i < (sizeof(HTTP_METHOD_TYPES) / sizeof(*HTTP_METHOD_TYPES)); i++)
	{
		const HTTP_METHOD_TYPE* type = &HTTP_METHOD_TYPES[i];
		if (text == type->name)
			return type->type;
	}
	return HTTP_NULL;
}