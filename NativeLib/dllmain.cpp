// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"

#include <iostream>
#include "MetaHost.h"

#pragma comment(lib, "mscoree.lib")

#ifdef __cplusplus
extern "C" {  
#endif

    __declspec(dllimport) void NativeFunctionInsideNativeLib();

#ifdef __cplusplus
}
#endif

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void __declspec(dllexport) __stdcall NativeFunctionInsideNativeLib()
{
    std::cout << "Inside NativeFunctionInsideNativeLib()!" << std::endl;

    ICLRMetaHost* metaHost = NULL;
    HRESULT hrCreateInstance = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&metaHost);

    if (hrCreateInstance == S_OK)
    {
        IEnumUnknown* installedRuntimes;
        HRESULT hrInstalledRuntimes = metaHost->EnumerateInstalledRuntimes(&installedRuntimes);
        if (hrInstalledRuntimes == S_OK)
        {
            while (true)
            {
                ICLRRuntimeInfo* runtimeInfo = NULL;
                ULONG fetched = 0;
                HRESULT hrNext = installedRuntimes->Next(1, (IUnknown**)&runtimeInfo, &fetched);
                if (hrNext == S_OK)
                {
                    if (fetched > 0)
                    {
                        wchar_t version[100];
                        DWORD versionLentgh = 100;
                        HRESULT hrVersion = runtimeInfo->GetVersionString(version, &versionLentgh);

                        // did we found runtime 4.0?
                        if (hrVersion == S_OK && versionLentgh >= 2 && version[1] == '4')
                        {
                            ICLRRuntimeHost* runtimeHost = NULL;
                            HRESULT hrInterface = runtimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (void**)&runtimeHost);
                            if (hrInterface == S_OK)
                            {
                                runtimeHost->Start();

                                DWORD returnValue;
                                HRESULT hrDomain = runtimeHost->ExecuteInDefaultAppDomain(L"ManagedLib.dll", L"ManagedLib.ManagedClass", L"ManagedMethod", L"test", &returnValue);
                                if (hrDomain != S_OK)
                                {
                                    std::cout << "Unable to run ManagedMethod of ManagedClass inside ManagedLib.dll!" << std::endl;
                                }
                                runtimeHost->Release();                                
                                break;
                            }
                        }
                    }
                    runtimeInfo->Release();
                }
                else
                {
                    std::cout << "Unable to get installed runtimes!" << std::endl;
                    break;
                }
            }
            installedRuntimes->Release();
        }        
        metaHost->Release();
    }
}

