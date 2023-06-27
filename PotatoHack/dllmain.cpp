// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <vector>

uintptr_t* ResolvePointerChain(uintptr_t next_addr, std::vector<uintptr_t> offsets)
{
    uintptr_t offset_addr = 0;
    for (int i = 0; i < offsets.size(); i++)
    {
		offset_addr = next_addr + offsets[i]; // Compute next address w/ offset
		next_addr = *(uintptr_t*)offset_addr; // Grab what's inside
    }
    return (uintptr_t*)offset_addr;
}

DWORD WINAPI Main(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    HANDLE potatoBase = GetModuleHandle(NULL);
    std::cout << "Module Base: " << std::hex << potatoBase << std::endl;

    std::vector<uintptr_t> offsets = { 
        0x278E510, 
        0x148,
        0x278,
        0xF8,
        0x58,
        0x20,
        0x178,
        0x58,
        0x20,
        0x8
    };
    int* health_ptr = (int*)ResolvePointerChain((uintptr_t)potatoBase, offsets);

    std::cout << "Health: " << std::dec << *health_ptr << std::endl;

    std::cout << "Ready for input" << std::endl;
    while (true)
    {
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            break;
        }
		*health_ptr = 20;
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	HANDLE threadHandle = NULL;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        threadHandle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, nullptr);
        if (threadHandle)
        {
            CloseHandle(threadHandle);
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

