// 1
// *(BYTE *)dwPatchAddr = 0xE8;
// *(DWORD *)(dwPatchAddr + 1) = (DWORD)((char *)myFunc - (char *)(dwPatchAddr + 5));

// 2
int  WINAPI MyMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uiType)
{
    return 0;
}

int _tmain(int argc, _TCHAR *argv[])
{
	//0x401000
	HMODULE hLib = LoadLibraryA("User32");

	if (hLib)
	{
		unsigned char* from = (unsigned char*)GetProcAddress(hLib, "MessageBoxW");
		if (from)
		{
			DWORD lpflOldProtect = PAGE_EXECUTE_READWRITE;

			VirtualProtect((LPVOID)from, 6, PAGE_EXECUTE_READWRITE, &lpflOldProtect);

			// direct 0xe9 jmp
			// unconditional jump opcode
			*from = 0xe9;

			// store the relative address from this opcode to our hook function
			*(unsigned long *)(from + 1) = (unsigned char *)&MyMessageBoxW - from - 5;

			VirtualProtect((LPVOID)from, 6, lpflOldProtect, NULL);
		}

		FreeLibrary(hLib);
	}

    MessageBoxW(0, 0, 0, 0);

    return 0;
}
