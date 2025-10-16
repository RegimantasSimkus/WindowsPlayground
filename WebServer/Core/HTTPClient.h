#pragma once
#include "HTTP.h"

namespace wpg
{
	namespace http
	{
		class ClientSession
		{
		public:
			ClientSession(SOCKET socket) : socket(socket)
			{
			}

			~ClientSession()
			{
				CloseConnection();
			}

			operator bool() const
			{
				return socket != INVALID_SOCKET;
			}

			int Receive(void* buff, int buffSize) const;

			std::string Receive(int buffSize = 0x1000) const;

			void SendRaw(std::string data) const;

			void SendText(std::string text, HTTP_FILE fileType = HTTP_FILE_PLAIN) const;

			void SendRaw(void* data, int size) const;

			void SendResponse(HTTPResponse& response) const;

			bool SendFile(std::string fileName, HTTP_FILE fileType) const;

			bool SendFile(std::string fileName) const;

			void SendQuickResponse(int status, std::string message) const;

			void CloseConnection();

			SOCKET socket;
		};
	};
};
