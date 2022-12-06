#pragma once

#include "ProfilerServer.h"
#include <d3d11_4.h>
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

#include <psapi.h>
#include <string>

float getRamUsage()
{
	//src: https://learn.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-process_memory_counters

	float memoryUsage = -1;

	DWORD currentProcessID = GetCurrentProcessId();

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, currentProcessID);

	if (NULL == hProcess)
		return -1;

	// https://learn.microsoft.com/en-us/windows/win32/psapi/process-memory-usage-information

	PROCESS_MEMORY_COUNTERS pmc{};
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		//PagefileUsage is the:
		//The Commit Charge value in bytes for this process.
		//Commit Charge is the total amount of memory that the memory manager has committed for a running process.

		memoryUsage = float(pmc.PagefileUsage / 1024.0 / 1024.0); //MiB
	}

	CloseHandle(hProcess);
	return memoryUsage;
}