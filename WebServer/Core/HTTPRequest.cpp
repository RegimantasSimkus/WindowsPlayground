#include "HTTPRequest.h"

using namespace wpg::http;

bool HTTPRequest::Parse()
{
	int methodLen = 0;
	int urlLen = 0;
	int versionLen = 0;

	this->method = _startline_segment(0, &methodLen);
	this->url = _startline_segment(1, &urlLen);
	this->version = _startline_segment(2, &versionLen);
	if (!this->method || !this->url || !this->version)
		return false;

	bool bodyNext = false;
	int bodyStart = -1;
	// locate everything
	for (int i = 1; ; i++)
	{
		int len = 0;
		char* line = _get_line(i, &len);
		if (!line)
			break;

		if (bodyNext)
		{
			// body
			body = line;
			break;
		}
		else if (!len)
		{
			// mark empty line for start of body
			bodyNext = true;
			bodyStart = i;
		}
		else
		{
			// headers
			char* pos = (char*)memchr(line, ':', len);
			if (!pos)
				continue;

			int headerTitleLen = (int)(pos - line);

			pos++; // skip the :
			if (*pos == ' ')
				pos++; // skip the ' ' if any

			int valueLen = (int)((line + len) - pos);
			headers[std::string(line, headerTitleLen)] = std::string(pos, valueLen);
		}
	}

	// terminate everything
	this->method[methodLen] = '\x00';
	this->url[urlLen] = '\x00';
	this->version[versionLen] = '\x00';
	return true;
}

char* HTTPRequest::_startline_segment(int index, int* lenOut)
{
	int curIndex = 0;
	char* cur = &message[0];
	char* start = cur;
	for (; *cur != '\n' && *cur != '\r' && *cur != '\x00'; cur++)
	{
		if (*cur == ' ')
		{
			if (index == curIndex)
				break;
			start = ++cur;
			curIndex++;
		}
	}

	if (curIndex != index)
		return nullptr;

	if (lenOut)
		*lenOut = (int)(cur - start);

	return start;
}

char* HTTPRequest::_get_line(int index, int* lenOut)
{
	int curIndex = 0;
	char* cur = &message[0];
	char* start = cur;

	for (; *cur != '\x00'; cur++)
	{
		if (*cur == '\n')
		{
			curIndex++;
			start = cur + 1;
		}
		else if (curIndex == index && (*cur == '\r' || *cur == '\x00'))
			break;
	}

	if (curIndex == index)
	{
		if (lenOut)
			*lenOut = (int)(cur - start);
		return start;
	}
	return nullptr;
}