#include "HTTPClient.h"
#include <vector>
#include <fstream>

using namespace wpg::http;

int ClientSession::Receive(void* buff, int buffSize) const
{
	int status = recv(socket, (char*)buff, buffSize, 0);
	return status;
}

std::string ClientSession::Receive(int buffSize) const
{
	std::vector<unsigned char> buffer(buffSize);
	int dataLen = Receive(buffer.data(), buffSize);
	if (!dataLen || dataLen == -1)
		return "";
	return std::string((const char*)buffer.data(), dataLen);
}

void ClientSession::SendRaw(std::string data) const
{
	send(socket, data.data(), (int)data.size(), 0);
}

void ClientSession::SendText(std::string text, HTTP_FILE fileType) const
{
	HTTPResponse res;
	res.SetContent(text, fileType);
	SendResponse(res);
}

void ClientSession::SendRaw(void* data, int size) const
{
	send(socket, (const char*)data, size, 0);
}

void ClientSession::SendResponse(HTTPResponse& response) const
{
	std::string res = response.BuildResponse();
	send(socket, res.data(), (int)res.size(), 0);
}

bool ClientSession::SendFile(std::string fileName, HTTP_FILE fileType) const
{
	std::ifstream stream(fileName.c_str(), std::ios_base::binary);
	if (!stream.is_open())
		return false;

	std::vector<char> buff(
		(std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>()
	);

	stream.close();

	HTTPResponse res(200, "OK");
	res.SetHeader("Content-Length", std::to_string(buff.size()));
	res.SetHeader("Content-Type", HTTPFileToMIME(fileType));
	SendResponse(res);
	SendRaw(buff.data(), (int)buff.size());
	return true;
}

bool ClientSession::SendFile(std::string fileName) const
{
	return SendFile(fileName, AssumeFileType(fileName));
}

void ClientSession::SendQuickResponse(int status, std::string message) const
{
	HTTPResponse res(status, message);
	return SendResponse(res);
}

void ClientSession::CloseConnection()
{
	if (socket)
		closesocket(socket);
	socket = NULL;
}