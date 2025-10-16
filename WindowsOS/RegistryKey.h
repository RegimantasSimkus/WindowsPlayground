#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

namespace wpg
{
	namespace windows
	{
		// A wrapper around HKEY that releases at deconstruction
		class KeyHandle
		{
		public:
			KeyHandle(HKEY hKey) : m_hKey(hKey)
			{
			}

			~KeyHandle()
			{
				Release();
			}

			// Move operators
			KeyHandle(KeyHandle&& other) noexcept : m_hKey(NULL)
			{
				m_hKey = other.m_hKey;
				other.m_hKey = NULL;
			}

			KeyHandle& operator=(KeyHandle&& other) noexcept
			{
				if (this != &other)
				{
					this->m_hKey = other.m_hKey;
					other.m_hKey = NULL;
				}
				return *this;
			}

			// Delete copy operators
			KeyHandle(const KeyHandle&) = delete;
			KeyHandle& operator=(const KeyHandle&) = delete;

			HKEY& operator*()
			{
				return m_hKey;
			}

			operator HKEY()
			{
				return m_hKey;
			}

			operator bool()
			{
				return m_hKey != NULL;
			}

			void Release()
			{
				if (m_hKey)
				{
					RegCloseKey(m_hKey);
					m_hKey = NULL;
				}
			}

			HKEY m_hKey;
		};

		class RegistryKey
		{
		private:

		public:
			RegistryKey(HKEY keyRoot, std::string regName) : m_regRoot(keyRoot), m_regName(regName)
			{

			}

			// Opens a registry key with an absolute registry path, including the root
			RegistryKey(std::string regName);

			~RegistryKey()
			{

			}

			bool IsValid() const;

			KeyHandle Open(REGSAM samDesired) const;

			// How many values under this reg key
			DWORD GetValueCount() const;

			// Returns the longest key-name-length (not name length) excluding terminator byte
			DWORD GetLongestValueNameLen() const;

			// Returns the longest key-value-length excluding terminator byte
			DWORD GetLongestValueLen() const;

			// Gets the nth subkey name, buffSize should be ... todo
			bool GetSubkeyName(int index, char* buff, int buffSize) const;

			bool GetSubkeyName(int index, std::string& nameOut) const;

			RegistryKey OpenSubkey(std::string name) const;

			DWORD GetSubkeyCount() const;

			DWORD GetLongestSubkeyName() const;

			// Gets the nth value name, size of buff should be GetLongestKeyValueNameLen()+1 to be safe.
			// nameLen should be set to the buffer size, out comes the actual size of the name
			bool GetValueName(int index, char* nameBuff, DWORD& nameLen) const;

			bool GetValueNameSize(int index, DWORD& sizeOut) const;

			bool GetValueData(int index, char* nameBuff, DWORD& nameLen, LPDWORD dataSize) const;

			bool GetValueDataSize(int index, DWORD& dataSize) const;

			bool GetValueDataType(int index, LPDWORD datatype) const;

			bool GetValueDataSize(int index, LPDWORD datasize) const;

			bool GetValueDataRaw(std::string valueName, void* buff, DWORD& buffSize, LPDWORD dataType) const;

			bool GetValueString(std::string valueName, std::string& valueOut) const;

			bool GetValueName(int index, std::string& valueOut) const;

			HKEY m_regRoot;
			std::string m_regName;
		};
	}
}