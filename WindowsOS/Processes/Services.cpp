#include "Services.h"

using namespace wpg;

bool windows::GetServices(std::vector<Service_t>& servicesOut, DWORD serviceType, DWORD serviceState)
{
	SC_HANDLE scManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (!scManager)
		return false;

	DWORD bytesNeeded = 0;
	DWORD serviceCount = 0;
	BOOL ret = EnumServicesStatusA(scManager, serviceType, serviceState, nullptr, NULL, &bytesNeeded, &serviceCount, nullptr);
	if (!ret && GetLastError() != ERROR_MORE_DATA)
		return false;

	std::vector<unsigned char> buffer(bytesNeeded);
	LPENUM_SERVICE_STATUSA serviceStatuses = (LPENUM_SERVICE_STATUSA)buffer.data();
	ret = EnumServicesStatusA(scManager, serviceType, serviceState, serviceStatuses, bytesNeeded, &bytesNeeded, &serviceCount, nullptr);
	if (!ret)
		return false;

	servicesOut.reserve(serviceCount);
	for (DWORD i = 0; i < serviceCount; i++)
	{
		ENUM_SERVICE_STATUSA& status = serviceStatuses[i];

		Service_t entry;
		entry.displayName = status.lpDisplayName;
		entry.serviceName = status.lpServiceName;
		entry.serviceType = status.ServiceStatus.dwServiceType;
		entry.currentState = status.ServiceStatus.dwCurrentState;
		entry.controlsAccepted = status.ServiceStatus.dwControlsAccepted;
		servicesOut.push_back(entry);
	}

	CloseServiceHandle(scManager);
	return true;
}