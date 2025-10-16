#include "PEInspect.h"
#include <iostream>

bool wpg::binaries::GetBinaryFileImports()
{
	// kernel32 test subject
	HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
	if (!kernel32)
		return false;

	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)kernel32;
	PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((unsigned char*)dos + dos->e_lfanew);

	if (nt->Signature != 0x4550)
		return false;

	printf("Signature: %s\n", &nt->Signature); // 0x00004550 ("PE\x00\x00")
	printf("AddressOfEntryPoint: %p\n", nt->OptionalHeader.AddressOfEntryPoint);

	PIMAGE_FILE_HEADER fileheader = &nt->FileHeader;
	printf("----[ Sections ]----\n");
	printf("- Num Sections: %d\n- Num Symbols: %d\n", fileheader->NumberOfSections, fileheader->NumberOfSymbols);

	// todo
}