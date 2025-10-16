#pragma once
#include "RegistryKey.h"
#include <vector>
#include <string>

namespace wpg
{
	namespace windows
	{
		struct SerialPortPair_t
		{
			SerialPortPair_t() : deviceName(), portName()
			{
			}

			SerialPortPair_t(std::string deviceName, std::string portName) : deviceName(deviceName), portName(portName)
			{
			}

			std::string deviceName;
			std::string portName;
		};

		struct WindowsInstalledApp_t
		{
			WindowsInstalledApp_t() : 
				displayName(), 
				installLocation(), 
				publisher(), 
				uninstallString(), 
				is32bit(false)
			{
			}

			WindowsInstalledApp_t(std::string displayName, std::string installLocation, std::string uninstallString, std::string publisher = "", bool is32bit = false) :
				displayName(displayName), 
				installLocation(installLocation), 
				uninstallString(uninstallString), 
				publisher(publisher), 
				is32bit(is32bit)
			{
			}

			std::string displayName;
			std::string installLocation;
			std::string uninstallString;
			std::string publisher;
			bool is32bit;
		};

		// Get available Serial ports using Registry Keys
		std::vector<SerialPortPair_t> GetAvailableSerialPorts();

		// Get installed apps from the 'Uninstall' list, using Registry Keys
		std::vector<WindowsInstalledApp_t> GetInstalledApps(bool x86Apps = true, bool x64Apps = true);

		void PrintRegistryKeyTree(const RegistryKey& key, int maxDepth, int depth);
	}
}