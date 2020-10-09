// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"
#include <set>
#include <fstream>
#include <tchar.h>
#include <list>
#include "dllmain.h"
#include "PluginLoadHook.h"
#include <thread>

static HINSTANCE hL;
static HMODULE gameModule;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  reason, LPVOID) {
    if(reason == DLL_PROCESS_ATTACH) {

		hL = LoadLibrary(L".\\d3d11_org.dll"); // Load our original d3d11 dll.
		if (!hL) { // This'll run if we're missing the original d3d11 dll or if its improper somehow.
			MessageBoxW(NULL, L"Please add the original d3d11 dll (`d3d11_org.dll`).", L"Plugin Loader", MB_OK | MB_ICONERROR);
			ExitProcess(1); // Close the current process since the game will just crash (later) if we return false
			return false;
		}

		gameModule = hModule;
		DisableThreadLibraryCalls(hModule);

        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)executionThread, NULL, NULL, NULL);
    }
	if(reason == DLL_PROCESS_DETACH) {
		FreeLibrary(hL); // Free our proxied d3d11 lib so that way we can die in peace
    }
	return TRUE;
}

int executionThread() {
    std::string cmdArgs = GetCommandLineA(); // Get the command line args for our running process

    // If we're running in debug mode, we wanna allocate the console
    if (cmdArgs.find("--debug") != std::string::npos) {
        AllocConsole(); // Allocate our console
    }

    SetConsoleTitle(L"Borderlands 3 Plugin Loader");

    FILE* f = nullptr;
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONOUT$", "w", stdout);

    HANDLE hStdout = CreateFile(L"CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hStdin = CreateFile(L"CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    SetStdHandle(STD_OUTPUT_HANDLE, hStdout); // Set our STD handles
    SetStdHandle(STD_ERROR_HANDLE, hStdout); // stderr is going back to STDOUT
    SetStdHandle(STD_INPUT_HANDLE, hStdin);

    std::wcout << "Console allocated...\n";
    std::wcout << "==== Debug ====\n";

    InitializePluginHooks(gameModule);

    return TRUE;
}


#pragma region Linker Exports

#pragma comment(linker, "/export:D3D11CoreRegisterLayers=d3d11_org.D3D11CoreRegisterLayers")

#pragma comment(linker, "/export:D3D11CoreGetLayeredDeviceSize=d3d11_org.D3D11CoreGetLayeredDeviceSize")

#pragma comment(linker, "/export:D3D11CoreCreateLayeredDevice=d3d11_org.D3D11CoreCreateLayeredDevice")

#pragma comment(linker, "/export:D3D11CoreCreateDevice=d3d11_org.D3D11CoreCreateDevice")

#pragma comment(linker, "/export:D3D11CreateDeviceAndSwapChain=d3d11_org.D3D11CreateDeviceAndSwapChain")

#pragma comment(linker, "/export:D3D11CreateDevice=d3d11_org.D3D11CreateDevice")

#pragma comment(linker, "/export:EnableFeatureLevelUpgrade=d3d11_org.EnableFeatureLevelUpgrade")

#pragma comment(linker, "/export:D3DKMTWaitForSynchronizationObject=d3d11_org.D3DKMTWaitForSynchronizationObject")

#pragma comment(linker, "/export:D3DKMTUnlock=d3d11_org.D3DKMTUnlock")

#pragma comment(linker, "/export:D3DKMTSignalSynchronizationObject=d3d11_org.D3DKMTSignalSynchronizationObject")

#pragma comment(linker, "/export:D3DKMTCloseAdapter=d3d11_org.D3DKMTCloseAdapter")

#pragma comment(linker, "/export:D3DKMTCreateAllocation=d3d11_org.D3DKMTCreateAllocation")

#pragma comment(linker, "/export:D3DKMTCreateContext=d3d11_org.D3DKMTCreateContext")

#pragma comment(linker, "/export:D3DKMTCreateDevice=d3d11_org.D3DKMTCreateDevice")

#pragma comment(linker, "/export:D3DKMTCreateSynchronizationObject=d3d11_org.D3DKMTCreateSynchronizationObject")

#pragma comment(linker, "/export:D3DKMTDestroyAllocation=d3d11_org.D3DKMTDestroyAllocation")

#pragma comment(linker, "/export:D3DKMTDestroyContext=d3d11_org.D3DKMTDestroyContext")

#pragma comment(linker, "/export:D3DKMTDestroyDevice=d3d11_org.D3DKMTDestroyDevice")

#pragma comment(linker, "/export:D3DKMTDestroySynchronizationObject=d3d11_org.D3DKMTDestroySynchronizationObject")

#pragma comment(linker, "/export:D3DKMTGetSharedPrimaryHandle=d3d11_org.D3DKMTGetSharedPrimaryHandle")

#pragma comment(linker, "/export:D3DKMTGetContextSchedulingPriority=d3d11_org.D3DKMTGetContextSchedulingPriority")

#pragma comment(linker, "/export:D3DKMTSetVidPnSourceOwner=d3d11_org.D3DKMTSetVidPnSourceOwner")

#pragma comment(linker, "/export:D3DKMTGetDisplayModeList=d3d11_org.D3DKMTGetDisplayModeList")

#pragma comment(linker, "/export:D3DKMTGetMultisampleMethodList=d3d11_org.D3DKMTGetMultisampleMethodList")

#pragma comment(linker, "/export:D3DKMTGetRuntimeData=d3d11_org.D3DKMTGetRuntimeData")

#pragma comment(linker, "/export:D3DKMTSetGammaRamp=d3d11_org.D3DKMTSetGammaRamp")

#pragma comment(linker, "/export:D3DKMTLock=d3d11_org.D3DKMTLock")

#pragma comment(linker, "/export:D3DKMTSetDisplayPrivateDriverFormat=d3d11_org.D3DKMTSetDisplayPrivateDriverFormat")

#pragma comment(linker, "/export:D3DKMTSetDisplayMode=d3d11_org.D3DKMTSetDisplayMode")

#pragma comment(linker, "/export:D3DKMTPresent=d3d11_org.D3DKMTPresent")

#pragma comment(linker, "/export:D3DKMTSetContextSchedulingPriority=d3d11_org.D3DKMTSetContextSchedulingPriority")

#pragma comment(linker, "/export:D3DKMTQueryAllocationResidency=d3d11_org.D3DKMTQueryAllocationResidency")

#pragma comment(linker, "/export:D3DKMTSetAllocationPriority=d3d11_org.D3DKMTSetAllocationPriority")

#pragma comment(linker, "/export:D3DKMTRender=d3d11_org.D3DKMTRender")

#pragma comment(linker, "/export:D3DKMTWaitForVerticalBlankEvent=d3d11_org.D3DKMTWaitForVerticalBlankEvent")

#pragma comment(linker, "/export:D3D11CreateDeviceForD3D12=d3d11_org.D3D11CreateDeviceForD3D12")

#pragma comment(linker, "/export:D3D11On12CreateDevice=d3d11_org.D3D11On12CreateDevice")

#pragma comment(linker, "/export:D3DPerformance_BeginEvent=d3d11_org.D3DPerformance_BeginEvent")

#pragma comment(linker, "/export:D3DPerformance_EndEvent=d3d11_org.D3DPerformance_EndEvent")

#pragma comment(linker, "/export:D3DPerformance_GetStatus=d3d11_org.D3DPerformance_GetStatus")

#pragma comment(linker, "/export:D3DPerformance_SetMarker=d3d11_org.D3DPerformance_SetMarker")

#pragma comment(linker, "/export:D3DKMTQueryAdapterInfo=d3d11_org.D3DKMTQueryAdapterInfo")

#pragma comment(linker, "/export:OpenAdapter10=d3d11_org.OpenAdapter10")

#pragma comment(linker, "/export:OpenAdapter10_2=d3d11_org.OpenAdapter10_2")

#pragma comment(linker, "/export:D3DKMTEscape=d3d11_org.D3DKMTEscape")

#pragma comment(linker, "/export:D3DKMTGetDeviceState=d3d11_org.D3DKMTGetDeviceState")

#pragma comment(linker, "/export:D3DKMTOpenAdapterFromHdc=d3d11_org.D3DKMTOpenAdapterFromHdc")

#pragma comment(linker, "/export:D3DKMTOpenResource=d3d11_org.D3DKMTOpenResource")

#pragma comment(linker, "/export:D3DKMTQueryResourceInfo=d3d11_org.D3DKMTQueryResourceInfo")

#pragma comment(linker, "/export:CreateDirect3D11DeviceFromDXGIDevice=d3d11_org.CreateDirect3D11DeviceFromDXGIDevice")

#pragma comment(linker, "/export:CreateDirect3D11SurfaceFromDXGISurface=d3d11_org.CreateDirect3D11SurfaceFromDXGISurface")

#pragma endregion
