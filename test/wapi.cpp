//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   wapi.cpp - prototypes WinAPI functions                             //
//   Created by:                                                        //
//   @nummer aka @nummerok                                              //
//   05.02.2016                                                         //
//                                                                      //
//   modified: 05.02.2016                                               //
//                                                                      //
//   undocumented WinAPI functions                                      //
//                                                                      //
//   https://github.com/Nummer/wapi                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "wapi.h"

namespace _wapi_api {
	void Log(const char* _Format, ...)
	{
#ifdef WAPI_FULL_LOG
		printf(_Format, new va_list);
#endif
	}

	HMODULE GetNTDLLHmodule()
	{
		HMODULE hDLL = GetModuleHandle(TEXT("ntdll.dll"));
		if (hDLL == NULL)
		{
			_wapi_api::Log("[-] Failed to find ntdll.dll\n");
			return NULL;
		}
		_wapi_api::Log("[+] Got ntdll.dll handle. Address --> 0x%x\n", (size_t)hDLL);
		return hDLL;
	}
}

namespace _wapi_ntdll {
	////////////////////////////////////////////////////////////////////
	// APC

	//----------------------------
	// KiUserApcDispatcher
	VOID NTAPI	KiUserApcDispatcher(
		IN PVOID                Unused1,
		IN PVOID                Unused2,
		IN PVOID                Unused3,
		IN PVOID                ContextStart,
		IN PVOID                ContextBody)
	{
		typedef VOID(NTAPI *KiUserApcDispatcherPrototype)(PVOID	Unused1, PVOID Unused2, PVOID Unused3, PVOID ContextStart, PVOID ContextBody);
		HMODULE hDLL = _wapi_api::GetNTDLLHmodule();
		if (hDLL == NULL)
			return;
		KiUserApcDispatcherPrototype UserApcDispatcher = (KiUserApcDispatcherPrototype)GetProcAddress(hDLL, "KiUserApcDispatcher");
		if (UserApcDispatcher == NULL)
		{
			_wapi_api::Log("[-] Failed to find KiUserApcDispatcher\n");
			return;
		}
		_wapi_api::Log("[+] Found KiUserApcDispatcher address. Address --> 0x%x\n", (size_t)UserApcDispatcher);
		_wapi_api::Log("[*] Calling KiUserApcDispatcher...\n");
		UserApcDispatcher(Unused1, Unused2, Unused3, ContextStart, ContextBody);
	}

	//----------------------------
	// NtAlertThread
	NTSTATUS NTAPI	NtAlertThread(
		IN HANDLE               ThreadHandle)
	{
		typedef NTSTATUS(NTAPI *NtAlertThreadPrototype)(HANDLE ThreadHandle);
		HMODULE hDLL = _wapi_api::GetNTDLLHmodule();
		if (hDLL == NULL)
			return NULL;
		NtAlertThreadPrototype ntAlertThread = (NtAlertThreadPrototype)GetProcAddress(hDLL, "NtAlertThread");
		if (ntAlertThread == NULL)
		{
			_wapi_api::Log("[-] Failed to find NtAlertThread\n");
			return NULL;
		}
		_wapi_api::Log("[+] Found NtAlertThread address. Address --> 0x%x\n", (size_t)ntAlertThread);
		_wapi_api::Log("[*] Calling NtAlertThread...\n");
		NTSTATUS result = ntAlertThread(ThreadHandle);
		_wapi_api::Log("[+] thread alerted state (returned) --> %d\n\n", result);
		return result;
	}

	//----------------------------
	// NtCallbackReturn
	NTSTATUS NTAPI NtCallbackReturn(
		IN PVOID                Result OPTIONAL,
		IN ULONG                ResultLength,
		IN NTSTATUS             Status)
	{
		typedef NTSTATUS(NTAPI *CallbackReturnPrototype)(PVOID Result OPTIONAL, ULONG ResultLength, NTSTATUS Status);
		HMODULE hDLL = _wapi_api::GetNTDLLHmodule();
		if (hDLL == NULL)
			return NULL;
		CallbackReturnPrototype ntCallbackReturn = (CallbackReturnPrototype)GetProcAddress(hDLL, "NtCallbackReturn");
		if (ntCallbackReturn == NULL)
		{
			_wapi_api::Log("[-] Failed to find NtCallbackReturn\n");
			return NULL;
		}
		_wapi_api::Log("[+] Found NtCallbackReturn address. Address --> 0x%x\n", (size_t)ntCallbackReturn);
		_wapi_api::Log("[*] Calling NtCallbackReturn...\n");
		NTSTATUS result = ntCallbackReturn(Result, ResultLength, Status);
		_wapi_api::Log("[+] returned --> %d\n\n", result);
		return result;
	}



	////////////////////////////////////////////////////////////////////
	// HASH

	//----------------------------
	// RtlComputeCrc32
	INT NTAPI RtlComputeCrc32(
		IN INT						accumCRC32,
		IN const BYTE*				buffer,
		IN UINT						buflen)
	{
		typedef INT(NTAPI *RtlComputeCrc32Prototype)(INT accumCRC32, const BYTE* buffer, UINT buflen);
		HMODULE hDLL = _wapi_api::GetNTDLLHmodule();
		if (hDLL == NULL)
			return NULL;
		RtlComputeCrc32Prototype ComputeCrc32 = (RtlComputeCrc32Prototype)GetProcAddress(hDLL, "RtlComputeCrc32");
		if (ComputeCrc32 == NULL)
		{
			_wapi_api::Log("[-] Failed to find RtlComputeCrc32\n");
			return NULL;
		}
		_wapi_api::Log("[+] Found RtlComputeCrc32 address. Address --> 0x%x\n", (size_t)ComputeCrc32);
		_wapi_api::Log("[*] Calling RtlComputeCrc32...\n");
		INT iCRC32 = ComputeCrc32(accumCRC32, (BYTE*)buffer, 3);
		_wapi_api::Log("[+] Computed CRC32 --> 0x%x\n\n", iCRC32);
		return iCRC32;
	}



	////////////////////////////////////////////////////////////////////
	// TIME

	//----------------------------
	// NtGetTickCount
	ULONG NTAPI NtGetTickCount()
	{
		typedef ULONG(NTAPI *NtGetTickCountPrototype)();
		HMODULE hDLL = _wapi_api::GetNTDLLHmodule();
		if (hDLL == NULL)
			return NULL;
		NtGetTickCountPrototype GetTickCount = (NtGetTickCountPrototype)GetProcAddress(hDLL, "NtGetTickCount");
		if (GetTickCount == NULL)
		{
			_wapi_api::Log("[-] Failed to find NtGetTickCount\n");
			return NULL;
		}
		_wapi_api::Log("[+] Found NtGetTickCount address. Address --> 0x%x\n", (size_t)GetTickCount);
		_wapi_api::Log("[*] Calling NtGetTickCount...\n");
		ULONG ticks = GetTickCount();
		_wapi_api::Log("[+] ticks --> %d\n\n", ticks);
		return ticks;
	}

	//----------------------------
	// NtSetTimerResolution
	NTSTATUS NTAPI NtSetTimerResolution(
		IN ULONG                DesiredResolution,
		IN BOOLEAN              SetResolution,
		OUT PULONG              CurrentResolution)
	{

		typedef NTSTATUS(NTAPI *NtSetTimerResolutionPrototype)(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);
		HMODULE hDLL = _wapi_api::GetNTDLLHmodule();
		if (hDLL == NULL)
			return NULL;
		NtSetTimerResolutionPrototype ntSetTimerResolution = (NtSetTimerResolutionPrototype)GetProcAddress(hDLL, "NtSetTimerResolution");
		if (ntSetTimerResolution == NULL)
		{
			_wapi_api::Log("[-] Failed to find NtSetTimerResolution\n");
			return NULL;
		}
		_wapi_api::Log("[+] Found NtSetTimerResolution address. Address --> 0x%x\n", (size_t)ntSetTimerResolution);
		_wapi_api::Log("[*] Calling NtSetTimerResolution...\n");
		NTSTATUS result = ntSetTimerResolution(DesiredResolution, SetResolution, CurrentResolution);
		_wapi_api::Log("[+] result --> %d\n\n", result);
		return result;
	}

	//----------------------------
	// NtQueryTimerResolution
	NTSTATUS NTAPI NtQueryTimerResolution(
		OUT PULONG              MinimumResolution,
		OUT PULONG              MaximumResolution,
		OUT PULONG              CurrentResolution)
	{

		typedef NTSTATUS(NTAPI *NtQueryTimerResolutionPrototype)(PULONG MinimumResolution, PULONG MaximumResolution, PULONG CurrentResolution);
		HMODULE hDLL = _wapi_api::GetNTDLLHmodule();
		if (hDLL == NULL)
			return NULL;
		NtQueryTimerResolutionPrototype ntQueryTimerResolution = (NtQueryTimerResolutionPrototype)GetProcAddress(hDLL, "NtSetTimerResolution");
		if (ntQueryTimerResolution == NULL)
		{
			_wapi_api::Log("[-] Failed to find NtQueryTimerResolution\n");
			return NULL;
		}
		_wapi_api::Log("[+] Found NtQueryTimerResolution address. Address --> 0x%x\n", (size_t)ntQueryTimerResolution);
		_wapi_api::Log("[*] Calling NtQueryTimerResolution...\n");
		NTSTATUS result = ntQueryTimerResolution(MinimumResolution, MaximumResolution, CurrentResolution);
		_wapi_api::Log("[+] result --> %d\n\n", result);
		return result;
	}
}