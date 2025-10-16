#include "RegistryKey.h"
#include <vector>

using namespace wpg::windows;

/*
	one of the following:

	HKEY_CLASSES_ROOT
	HKEY_CURRENT_CONFIG
	HKEY_CURRENT_USER
	HKEY_LOCAL_MACHINE
	HKEY_USERS 
*/
static const struct { HKEY root; const char* name; } REGISTRY_KEY_ROOT_MAP[] = {
	{HKEY_CLASSES_ROOT, "HKEY_CLASSES_ROOT"},
	{HKEY_CURRENT_CONFIG, "HKEY_CURRENT_CONFIG"},
	{HKEY_CURRENT_USER, "HKEY_CURRENT_USER"},
	{HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE"},
	{HKEY_USERS, "HKEY_USERS"},
};

RegistryKey::RegistryKey(std::string regName)
{
	// HKEY_CLASSES_ROOT = 0
	// default
	m_regRoot = 0;
	m_regName = regName;

	int rootEnd = regName.find_first_of('\\');
	if (rootEnd == std::string::npos)
		return;

	std::string rootStr = regName.substr(0, rootEnd);
	for (int i = 0; i < (sizeof(REGISTRY_KEY_ROOT_MAP) / sizeof(*REGISTRY_KEY_ROOT_MAP)); i++)
	{
		auto* entry = &REGISTRY_KEY_ROOT_MAP[i];
		if (rootStr == entry->name)
		{
			m_regRoot = entry->root;
			m_regName = regName.substr(rootEnd + 1);
			break;
		}
	}
}

bool RegistryKey::IsValid() const
{
	return Open(KEY_QUERY_VALUE) != 0;
}

KeyHandle RegistryKey::Open(REGSAM samDesired) const
{
	HKEY key = 0;
	LSTATUS result = RegOpenKeyExA(m_regRoot, m_regName.c_str(), 0, samDesired, &key);
	if (result == ERROR_SUCCESS)
		return key;
	return 0;
}

// How many values under this reg key
DWORD RegistryKey::GetValueCount() const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return 0;

	DWORD keyCount = 0;
	RegQueryInfoKeyA(key, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &keyCount, nullptr, nullptr, nullptr, nullptr);

	return keyCount;
}

// Returns the longest key-name-length (not name length) excluding terminator byte
DWORD RegistryKey::GetLongestValueNameLen() const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return 0;

	DWORD longestKeyLen = 0;
	RegQueryInfoKeyA(key, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &longestKeyLen, nullptr, nullptr, nullptr);

	return longestKeyLen;
}

// Returns the longest key-value-length excluding terminator byte
DWORD RegistryKey::GetLongestValueLen() const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return 0;

	DWORD longestKeyLen = 0;
	RegQueryInfoKeyA(key, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &longestKeyLen, nullptr, nullptr);

	return longestKeyLen;
}

// Gets the nth subkey name, buffSize should be ... todo
bool RegistryKey::GetSubkeyName(int index, char* buff, int buffSize) const
{
	KeyHandle key = Open(KEY_ENUMERATE_SUB_KEYS);
	if (!key)
		return false;

	LSTATUS result = RegEnumKeyA(key, index, buff, buffSize);

	return result == ERROR_SUCCESS;
}

bool RegistryKey::GetSubkeyName(int index, std::string& nameOut) const
{
	KeyHandle key = Open(KEY_ENUMERATE_SUB_KEYS);
	if (!key)
		return false;

	DWORD buffSize = GetLongestSubkeyName() + 1;
	std::vector<char> buffer(buffSize);

	LSTATUS result = RegEnumKeyA(key, index, buffer.data(), buffSize);
	if (result == ERROR_SUCCESS)
	{
		nameOut = buffer.data();
		return true;
	}
	return false;
}

RegistryKey RegistryKey::OpenSubkey(std::string name) const
{
	if ((!m_regName.empty() && m_regName.back() != '\\') && (!name.empty() && name.front() != '\\'))
		name = "\\" + name;
	return RegistryKey(m_regRoot, m_regName + name);
}

DWORD RegistryKey::GetSubkeyCount() const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return 0;

	DWORD keyCount = 0;
	RegQueryInfoKeyA(key, nullptr, nullptr, nullptr, &keyCount, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

	return keyCount;
}

DWORD RegistryKey::GetLongestSubkeyName() const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return 0;

	DWORD maxLen = 0;
	RegQueryInfoKeyA(key, nullptr, nullptr, nullptr, nullptr, &maxLen, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

	return maxLen;
}

// Gets the nth value name, size of buff should be GetLongestKeyValueNameLen()+1 to be safe.
// nameLen should be set to the buffer size, out comes the actual size of the name
bool RegistryKey::GetValueName(int index, char* nameBuff, DWORD& nameLen) const //, PVOID dataBuff, LPDWORD dataBuffSize, LPDWORD datatype, LPDWORD datasize)
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return false;

	LSTATUS result = RegEnumValueA(key, index, nameBuff, &nameLen, NULL, nullptr, nullptr, nullptr); // datatype, (LPBYTE)dataBuff, dataBuffSize);
	return result == ERROR_SUCCESS;
}

bool RegistryKey::GetValueNameSize(int index, DWORD& sizeOut) const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return false;

	DWORD nameLen = 0;

	LSTATUS result = RegEnumValueA(key, index, nullptr, &nameLen, nullptr, nullptr, nullptr, &sizeOut);
	return result == ERROR_SUCCESS;
}

bool RegistryKey::GetValueData(int index, char* nameBuff, DWORD& nameLen, LPDWORD dataSize) const //, PVOID dataBuff, LPDWORD dataBuffSize, LPDWORD datatype, LPDWORD datasize)
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return false;

	LSTATUS result = RegEnumValueA(key, index, nameBuff, &nameLen, NULL, nullptr, nullptr, dataSize); // datatype, (LPBYTE)dataBuff, dataBuffSize);

	return result == ERROR_SUCCESS;
}

bool RegistryKey::GetValueDataSize(int index, DWORD& dataSize) const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return false;

	DWORD nameLen = 1;
	CHAR name = 0;
	LSTATUS result = RegEnumValueA(key, index, &name, &nameLen, NULL, &nameLen, nullptr, &dataSize);

	return true;
}

bool RegistryKey::GetValueDataType(int index, LPDWORD datatype) const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return false;

	char dummyName;
	DWORD dummyLen = 1;
	LSTATUS result = RegEnumValueA(key, index, &dummyName, &dummyLen, NULL, datatype, nullptr, nullptr);

	return result == ERROR_SUCCESS || result == ERROR_MORE_DATA;
}

bool RegistryKey::GetValueDataSize(int index, LPDWORD dataSize) const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return false;

	char dummyName;
	DWORD dummyLen = 1;
	LSTATUS result = RegEnumValueA(key, index, &dummyName, &dummyLen, NULL, nullptr, nullptr, dataSize);

	return result == ERROR_SUCCESS || result == ERROR_MORE_DATA;
}

bool RegistryKey::GetValueDataRaw(std::string valueName, void* buff, DWORD& buffSize, LPDWORD dataType) const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return false;

	LSTATUS result = RegGetValueA(key, nullptr, valueName.c_str(), RRF_RT_ANY, dataType, buff, &buffSize);

	return result == ERROR_SUCCESS || result == ERROR_MORE_DATA;
}

bool RegistryKey::GetValueString(std::string valueName, std::string& valueOut) const
{
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return false;

	DWORD buffSize = GetLongestValueLen();
	std::vector<char> buffer(buffSize);

	LSTATUS result = RegGetValueA(key, nullptr, valueName.c_str(), RRF_RT_REG_SZ, nullptr, buffer.data(), &buffSize);

	if (result == ERROR_SUCCESS || result == ERROR_MORE_DATA)
	{
		valueOut = std::string(buffer.data(), buffSize - 1);
		return true;
	}
	return false;
}

bool RegistryKey::GetValueName(int index, std::string& valueOut) const
{
	valueOut.clear();
	KeyHandle key = Open(KEY_QUERY_VALUE);
	if (!key)
		return false;

	DWORD nameLen = GetLongestValueNameLen() + 1; // +1 to fit terminator
	std::vector<char> buffer(nameLen);

	LSTATUS result = RegEnumValueA(key, index, buffer.data(), &nameLen, NULL, nullptr, nullptr, nullptr); // datatype, (LPBYTE)dataBuff, dataBuffSize);

	if (result == ERROR_SUCCESS)
	{
		valueOut = std::string(buffer.data(), nameLen);
		return true;
	}
	return false;
}