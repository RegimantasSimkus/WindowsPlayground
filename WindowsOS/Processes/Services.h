#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include <string>

namespace wpg
{
	namespace windows
	{
		// Members populated from https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-enum_service_statusa
		struct Service_t
		{
			Service_t()
			{
			}

			std::string displayName;
			std::string serviceName;
			DWORD serviceType = 0;
			DWORD currentState = 0;
			DWORD controlsAccepted = 0;
		};

		// Uses internally - https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-enumservicesstatusa
		// parameters dwServiceType and dwServiceState are as used by EnumServiceStatusA
		bool GetServices(std::vector<Service_t>& servicesOut, DWORD serviceType = SERVICE_DRIVER | SERVICE_WIN32, DWORD serviceState = SERVICE_STATE_ALL);
	}
}