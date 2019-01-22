//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   wapi.h - prototypes WinAPI functions                               //
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

#pragma once

#undef WAPI_FULL_LOG


#include <Windows.h>

typedef LONG NTSTATUS;


#if ((defined(_DEBUG) || defined(DEBUG)) && !defined(WAPI_NO_LOG))
#define WAPI_FULL_LOG
#include <stdio.h>
#endif

namespace _wapi_api {
	HMODULE GetNTDLLHmodule();
}

namespace _wapi_ntdll {

	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	typedef enum _NT_PRODUCT_TYPE
	{
		NtProductWinNt = 1,
		NtProductLanManNt = 2,
		NtProductServer = 3
	} NT_PRODUCT_TYPE;

	typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE
	{
		StandardDesign = 0,
		NEC98x86 = 1,
		EndAlternatives = 2
	} ALTERNATIVE_ARCHITECTURE_TYPE;

	typedef struct _KSYSTEM_TIME
	{
		ULONG LowPart;
		LONG High1Time;
		LONG High2Time;
	} KSYSTEM_TIME, *PKSYSTEM_TIME;

	typedef struct _KUSER_SHARED_DATA
	{
		ULONG TickCountLowDeprecated;
		ULONG TickCountMultiplier;
		KSYSTEM_TIME InterruptTime;
		KSYSTEM_TIME SystemTime;
		KSYSTEM_TIME TimeZoneBias;
		WORD ImageNumberLow;
		WORD ImageNumberHigh;
		WCHAR NtSystemRoot[260];
		ULONG MaxStackTraceDepth;
		ULONG CryptoExponent;
		ULONG TimeZoneId;
		ULONG LargePageMinimum;
		ULONG Reserved2[7];
		NT_PRODUCT_TYPE NtProductType;
		UCHAR ProductTypeIsValid;
		ULONG NtMajorVersion;
		ULONG NtMinorVersion;
		UCHAR ProcessorFeatures[64];
		ULONG Reserved1;
		ULONG Reserved3;
		ULONG TimeSlip;
		ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture;
		LARGE_INTEGER SystemExpirationDate;
		ULONG SuiteMask;
		UCHAR KdDebuggerEnabled;
		UCHAR NXSupportPolicy;
		ULONG ActiveConsoleId;
		ULONG DismountCount;
		ULONG ComPlusPackage;
		ULONG LastSystemRITEventTickCount;
		ULONG NumberOfPhysicalPages;
		UCHAR SafeBootMode;
		ULONG SharedDataFlags;
		ULONG DbgErrorPortPresent : 1;
		ULONG DbgElevationEnabled : 1;
		ULONG DbgVirtEnabled : 1;
		ULONG DbgInstallerDetectEnabled : 1;
		ULONG SystemDllRelocated : 1;
		ULONG SpareBits : 27;
		UINT64 TestRetInstruction;
		ULONG SystemCall;
		ULONG SystemCallReturn;
		UINT64 SystemCallPad[3];
		union
		{
			KSYSTEM_TIME TickCount;
			UINT64 TickCountQuad;
		};
		ULONG Cookie;
		INT64 ConsoleSessionForegroundProcessId;
		ULONG Wow64SharedInformation[16];
		WORD UserModeGlobalLogger[8];
		ULONG HeapTracingPid[2];
		ULONG CritSecTracingPid[2];
		ULONG ImageFileExecutionOptions;
		union
		{
			UINT64 AffinityPad;
			ULONG ActiveProcessorAffinity;
		};
		UINT64 InterruptTimeBias;
	} KUSER_SHARED_DATA, *PKUSER_SHARED_DATA;
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------


	////////////////////////////////////////////////////////////////////
	// APC

	//----------------------------
	// KiUserApcDispatcher
	NTSYSAPI VOID NTAPI	KiUserApcDispatcher(
			IN PVOID                Unused1,
			IN PVOID                Unused2,
			IN PVOID                Unused3,
			IN PVOID                ContextStart,
			IN PVOID                ContextBody);

	//----------------------------
	// NtAlertThread
	NTSYSAPI NTSTATUS NTAPI	NtAlertThread(
			IN HANDLE               ThreadHandle);

	//----------------------------
	// NtCallbackReturn
	NTSYSAPI NTSTATUS NTAPI NtCallbackReturn(
			IN PVOID                Result OPTIONAL,
			IN ULONG                ResultLength,
			IN NTSTATUS             Status);



	////////////////////////////////////////////////////////////////////
	// HASH

	//----------------------------
	// RtlComputeCrc32
	NTSYSAPI INT NTAPI RtlComputeCrc32(
		IN INT						accumCRC32,
		IN const BYTE*				buffer,
		IN UINT						buflen);



	////////////////////////////////////////////////////////////////////
	// TIME

	//----------------------------
	// NtGetTickCount
	NTSYSAPI ULONG NTAPI NtGetTickCount();

	//----------------------------
	// NtSetTimerResolution
	NTSYSAPI NTSTATUS NTAPI NtSetTimerResolution(
			IN ULONG                DesiredResolution,
			IN BOOLEAN              SetResolution,
			OUT PULONG              CurrentResolution);

	//----------------------------
	// NtQueryTimerResolution
	NTSYSAPI NTSTATUS NTAPI NtQueryTimerResolution(
			OUT PULONG              MinimumResolution,
			OUT PULONG              MaximumResolution,
			OUT PULONG              CurrentResolution);

}