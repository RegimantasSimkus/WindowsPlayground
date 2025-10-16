#include "RegistryProcedures.h"
#include "RegistryKey.h"

using namespace wpg::windows;

// Fetch available serial ports using Windows Registry keys.
std::vector<SerialPortPair_t> wpg::windows::GetAvailableSerialPorts()
{
	const RegistryKey regKey(HKEY_LOCAL_MACHINE, R"(HARDWARE\DEVICEMAP\SERIALCOMM)");
	DWORD valueCount = regKey.GetValueCount();
	
	std::vector<SerialPortPair_t> availablePorts;
	availablePorts.reserve(valueCount);

	for (DWORD i = 0; i < valueCount; i++)
	{
		std::string deviceName, portName;

		if (!regKey.GetValueName(i, deviceName))
			continue;

		if (!regKey.GetValueString(deviceName, portName))
			continue;
		
		availablePorts.emplace_back(deviceName, portName);
	}

	return availablePorts;
}

// Enumerates the registry 'registryName' and populates the 'appList' with the 32bit flag set correspondingly
static void EnumerateUninstallList(const char* registryName, std::vector<WindowsInstalledApp_t>& appList, bool is32bit)
{
	const RegistryKey regKey(HKEY_LOCAL_MACHINE, registryName);
	DWORD subkeyCount = regKey.GetSubkeyCount();

	appList.reserve(appList.capacity() + subkeyCount);

	for (DWORD i = 0; i < subkeyCount; i++)
	{
		std::string subkeyName;
		regKey.GetSubkeyName(i, subkeyName);

		RegistryKey subkey = regKey.OpenSubkey(subkeyName);

		std::string displayName, publisher;
		std::string installLocation, uninstallString;
		if (!subkey.GetValueString("DisplayName", displayName))
			continue;

		if (!subkey.GetValueString("InstallLocation", installLocation))
			continue;

		if (!subkey.GetValueString("UninstallString", uninstallString))
			continue;

		subkey.GetValueString("Publisher", publisher);
		appList.emplace_back(displayName, installLocation, uninstallString, publisher, is32bit);
	}
}

// Returns a list of installed windows apps that are 'uninstallable'
std::vector<WindowsInstalledApp_t> wpg::windows::GetInstalledApps(bool x86Apps, bool x64Apps)
{
	std::vector<WindowsInstalledApp_t> installedApps;
	if (x64Apps)
		// x86/32bit uninstall location
		EnumerateUninstallList(R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall)", installedApps, false);

	if (x86Apps)
		// x64/64bit uninstall location
		EnumerateUninstallList(R"(SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall)", installedApps, true);

	return installedApps;
}

static inline std::string generateIndent(int n)
{
	return std::string(n, '\t');
}

void wpg::windows::PrintRegistryKeyTree(const RegistryKey& key, int maxDepth, int depth)
{
	if (maxDepth != -1 && depth >= maxDepth)
		return;

	DWORD subkeyCount = key.GetSubkeyCount();
	std::string indent = generateIndent(depth);
	for (int i = 0; i < subkeyCount; i++)
	{
		std::string subkeyName;
		if (!key.GetSubkeyName(i, subkeyName))
			continue;

		printf("%s[%s]\n", indent.c_str(), subkeyName.c_str());
		
		const RegistryKey subkey = key.OpenSubkey(subkeyName);
		if (subkey.IsValid() && depth < maxDepth)
		{
			PrintRegistryKeyTree(subkey, maxDepth, depth + 1);
		}
	}
}