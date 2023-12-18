#pragma once

//Project headers
#include "main.h"

//macro
#define SineWaveMacro(A, f, x, B) A*sin(3.6f*M_PI*f*x+B)

//function prototypes

VOID WINAPI fnPCMplayer(
	_In_reads_bytes_(iBufLength) CONST CHAR lBuffer[],
	_In_ CONST UINT uiFreq,
	_In_ CONST SIZE_T uiBufLength
);

VOID WINAPI fnNoise1(VOID);
VOID WINAPI fnNoise2(VOID);
VOID WINAPI fnSystemSounds(VOID);