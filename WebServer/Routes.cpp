#include "WebServer.h"
#include "../FileFormats/JSON.h"
#include "../WindowsOS/RegistryProcedures.h"

using namespace wpg::http;

HTTPRoute MemoryInfoRoute(HTTP_GET, "/api/meminfo", [](ClientSession* session, HTTPRequest& msg) -> bool
	{
		MEMORYSTATUSEX memstatus{};
		memstatus.dwLength = sizeof(memstatus);
		if (!GlobalMemoryStatusEx(&memstatus))
		{
			session->SendQuickResponse(500, "Error");
			return false;
		}

		std::ostringstream oss;
		oss << "{"
			<< "\"totalphys\": " << memstatus.ullTotalPhys << ","
			<< "\"availphys\": " << memstatus.ullAvailPhys << ","
			<< "\"memoryload\": " << memstatus.dwMemoryLoad
			<< "}";

		session->SendText(oss.str(), HTTP_FILE_JSON);
		return true;
	});

HTTPRoute MessageBoxRoute(HTTP_POST, "/api/messagebox", [](ClientSession* session, HTTPRequest& msg) -> bool
	{
		MessageBoxA(NULL, msg.body, "Windows Playground", MB_OK);
		session->SendQuickResponse(200, "OK");
		return true;
	});

HTTPRoute ScreenResolutionRoute(HTTP_GET, "/api/resolution", [](ClientSession* session, HTTPRequest& msg) -> bool
	{
		int scrw = GetSystemMetrics(SM_CXSCREEN);
		int scrh = GetSystemMetrics(SM_CYSCREEN);

		std::ostringstream oss;
		oss << "{"
			<< "\"scrw\": " << scrw << ","
			<< "\"scrh\": " << scrh
			<< "}";

		session->SendText(oss.str(), HTTP_FILE_JSON);
		return true;
	});

HTTPRoute SystemInfoRoute(HTTP_GET, "/api/system", [](ClientSession* session, HTTPRequest& msg) -> bool
	{
		SYSTEM_POWER_STATUS powerStatus;
		GetSystemPowerStatus(&powerStatus);
		ULONGLONG uptime = GetTickCount64();

		std::ostringstream oss;
		oss << "{"
			<< "\"uptime\": " << uptime << ","
			<< "\"battery_duration\": " << powerStatus.BatteryLifeTime << ","
			<< "\"battery_percentage\": " << (int)powerStatus.BatteryLifePercent
			<< "}";
		
		session->SendText(oss.str(), HTTP_FILE_JSON);
		return true;
	});

HTTPRoute UserInfoRoute(HTTP_GET, "/api/userinfo", [](ClientSession* session, HTTPRequest& msg) -> bool
	{
		char name[512];
		DWORD nameSize = sizeof(name);
		if (!GetUserNameA(name, &nameSize))
			return false;


		std::ostringstream oss;
		oss << "{"
			<< "\"username\": \"" << name << "\""
			<< "}";

		session->SendText(oss.str(), HTTP_FILE_JSON);
		return true;
	});

HTTPRoute GetAvailableSerialPortsRoute(HTTP_GET, "/api/serialports", [](ClientSession* session, HTTPRequest& msg) -> bool
	{
		wpg::fileformats::SimpleJSONListBuilder json;
		std::vector<wpg::windows::SerialPortPair_t> ports = wpg::windows::GetAvailableSerialPorts();

		for (auto& port : ports)
		{
			wpg::fileformats::SimpleJSONListEntry entry;
			entry.SetValue("DeviceName", port.deviceName);
			entry.SetValue("PortName", port.portName);
			json.AddEntry(entry);
		}

		session->SendText(json.Build(), HTTP_FILE_JSON);
		return true;
	});

HTTPRoute GetInstalledAppsRoute(HTTP_GET, "/api/installedapps", [](ClientSession* session, HTTPRequest& msg) -> bool
	{
		wpg::fileformats::SimpleJSONListBuilder json;
		std::vector<wpg::windows::WindowsInstalledApp_t> apps = wpg::windows::GetInstalledApps();

		for (auto& app : apps)
		{
			wpg::fileformats::SimpleJSONListEntry entry;
			entry.SetValue("DisplayName", app.displayName);
			entry.SetValue("InstallLocation", app.installLocation);
			entry.SetValue("UninstallString", app.uninstallString);
			json.AddEntry(entry);
		}

		session->SendText(json.Build(), HTTP_FILE_JSON);
		return true;
	});

HTTPRoute UninstallAppRoute(HTTP_POST, "/api/uninstallapp", [](ClientSession* session, HTTPRequest& msg) -> bool
	{
		std::string requestedUninstall = msg.body;
		std::vector<wpg::windows::WindowsInstalledApp_t> apps = wpg::windows::GetInstalledApps();

		for (auto& app : apps)
		{
			// comparing uninstallStrings so that not any message body can be sent & executed
			if (requestedUninstall == app.uninstallString)
			{
				printf("Uninstalling App \"%s\", published by \"%s\"\nCmd - %s\n", 
					app.displayName.c_str(), 
					app.publisher.c_str(), 
					app.uninstallString.c_str()
				);

				system(app.uninstallString.c_str());
				session->SendQuickResponse(200, "OK");
				return true;
			}
		}

		return false;
	});