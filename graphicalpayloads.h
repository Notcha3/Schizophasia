//#pragma once

//Project headers
#include "main.h"

extern HWND //g_hLayeredWindow,
	g_hRootWindow;

extern HDC //g_hLayeredWindowDC,
	g_hRootWindowDC;

extern RECT stScreenCords;

//Function prototypes

/*
VOID WINAPI fnCreateLayeredWindow(VOID);


LRESULT CALLBACK fnWindProc(
	_In_ HWND hWnd,
	_In_ UINT uiMessages,
	_Inout_ WPARAM pWparam,
	_In_ LPARAM lpParam
);
*/

BOOL CALLBACK fnEnumWindowsProc(
	_In_ HWND   hwnd,
    _In_ LPARAM lParam
);

VOID WINAPI fnWindTextRandomInt(VOID);
VOID WINAPI fnScreenRotation(VOID);
VOID WINAPI fnRandomPatternBrush(VOID);
VOID WINAPI fnSolidBrushColours(VOID);
VOID WINAPI fnHSLRandColourPal(VOID);
VOID WINAPI fnIncrementColourPalStatic(VOID);
VOID WINAPI fnIncrementColourPal(VOID);
VOID WINAPI fnCaptchaText(VOID);