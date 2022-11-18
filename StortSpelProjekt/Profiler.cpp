#pragma once
#include "stdafx.h"
#include "Profiler.h"
#include <d3d11_4.h>
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

#include <psapi.h>
#include <string>

void vramUsage()
{
	IDXGIFactory* dxgifactory = nullptr;
	HRESULT ret_code = ::CreateDXGIFactory(
		__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(&dxgifactory));

	if (SUCCEEDED(ret_code))
	{
		IDXGIAdapter* dxgiAdapter = nullptr;

		if (SUCCEEDED(dxgifactory->EnumAdapters(0, &dxgiAdapter)))
		{
			IDXGIAdapter4* dxgiAdapter4 = NULL;
			if (SUCCEEDED(dxgiAdapter->QueryInterface(__uuidof(IDXGIAdapter4), (void**)&dxgiAdapter4)))
			{
				DXGI_QUERY_VIDEO_MEMORY_INFO info;

				if (SUCCEEDED(dxgiAdapter4->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info)))
				{
					float memoryUsage = float(info.CurrentUsage / 1024.0 / 1024.0); //MiB

					char msg[100];
					sprintf_s(msg, "%.2f MiB used", memoryUsage);
					MessageBoxA(0, msg, "VRAM", 0);
				};

				dxgiAdapter4->Release();
			}
			dxgiAdapter->Release();
		}
		dxgifactory->Release();
	}
}

float getVramUsage()
{
	IDXGIFactory* dxgifactory = nullptr;
	HRESULT ret_code = ::CreateDXGIFactory(
		__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(&dxgifactory));
	float memoryUsage = -1;
	if (SUCCEEDED(ret_code))
	{
		IDXGIAdapter* dxgiAdapter = nullptr;

		if (SUCCEEDED(dxgifactory->EnumAdapters(0, &dxgiAdapter)))
		{
			IDXGIAdapter4* dxgiAdapter4 = NULL;
			if (SUCCEEDED(dxgiAdapter->QueryInterface(__uuidof(IDXGIAdapter4), (void**)&dxgiAdapter4)))
			{
				DXGI_QUERY_VIDEO_MEMORY_INFO info;

				if (SUCCEEDED(dxgiAdapter4->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info)))
				{
					memoryUsage = float(info.CurrentUsage / 1024.0 / 1024.0); //MiB
				};

				dxgiAdapter4->Release();
			}
			dxgiAdapter->Release();
		}
		dxgifactory->Release();
	}
	return memoryUsage;
}

void ramUsage()
{
	//src: https://learn.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-process_memory_counters

	DWORD currentProcessID = GetCurrentProcessId();

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, currentProcessID);

	if (NULL == hProcess)
		return;

	// https://learn.microsoft.com/en-us/windows/win32/psapi/process-memory-usage-information

	PROCESS_MEMORY_COUNTERS pmc{};
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		//PagefileUsage is the:
		//The Commit Charge value in bytes for this process.
		//Commit Charge is the total amount of memory that the memory manager has committed for a running process.

		float memoryUsage = float(pmc.PagefileUsage / 1024.0 / 1024.0); //MiB

		char msg[100];
		sprintf_s(msg, "%.2f MiB committed", memoryUsage);
		MessageBoxA(0, msg, "RAM", 0);
	}

	CloseHandle(hProcess);
}

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