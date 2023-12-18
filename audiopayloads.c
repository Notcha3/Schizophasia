#include "audiopayloads.h"


VOID WINAPI fnPCMplayer(
	_In_reads_bytes_(iBufLength) CONST CHAR lBuffer[],
	_In_ CONST UINT uiFreq,
	_In_ CONST SIZE_T uiBufLength
) {


	WAVEFORMATEX stWaveFmt = {0};

	stWaveFmt.nChannels = 1;
	stWaveFmt.nSamplesPerSec = uiFreq;
	stWaveFmt.wFormatTag = WAVE_FORMAT_PCM;
	stWaveFmt.cbSize = 0;
	stWaveFmt.wBitsPerSample =	8;
	stWaveFmt.nBlockAlign = (stWaveFmt.nChannels * stWaveFmt.wBitsPerSample)/8;
	stWaveFmt.nAvgBytesPerSec = stWaveFmt.nSamplesPerSec;


	WAVEHDR stWaveHeader = {NULL};

	stWaveHeader.dwBufferLength = 0;
	stWaveHeader.dwFlags = 0;
	stWaveHeader.dwLoops = 0;
	stWaveHeader.dwUser = 0;
	stWaveHeader.reserved = 0;
	stWaveHeader.lpData = NULL;
	stWaveHeader.lpNext = NULL;
	stWaveHeader.dwBytesRecorded = 0;

	HWAVEOUT hWaveDev = NULL;

		if (waveOutOpen(
		&hWaveDev,
		WAVE_MAPPER,
		&stWaveFmt,
		(DWORD_PTR)NULL,
		(DWORD_PTR)NULL,
		CALLBACK_NULL
		) != MMSYSERR_NOERROR) 

			MessageBoxW(
				NULL,
				g_alpszMessageBoxTexts[5],
				g_alpszTitles[1],
				MB_OK | MB_ICONERROR);
		

		stWaveHeader.lpData = (LPSTR) HeapAlloc(
			GetProcessHeap(), 
			HEAP_ZERO_MEMORY, 
			uiBufLength
			);

		if (stWaveHeader.lpData == NULL) {
			return;
			}
		//if memory allocation fails, we stop the function so it doesn't crash the whole malware
		for (INT iA = 0; iA < uiBufLength; iA++)
			stWaveHeader.lpData[iA] = lBuffer[iA];

		//copy the buffer into our newly allocated memory 

		stWaveHeader.dwBufferLength = uiBufLength;

		if (waveOutPrepareHeader(
			hWaveDev,
			&stWaveHeader,
			sizeof(stWaveHeader)
			) != MMSYSERR_NOERROR) 

			MessageBoxW(
				NULL, 
				g_alpszMessageBoxTexts[5],
				g_alpszTitles[1],
				MB_OK | MB_ICONERROR
			);
		

		if (waveOutWrite(
			hWaveDev,
			&stWaveHeader,
			sizeof(stWaveHeader)
		)	 != MMSYSERR_NOERROR)

			MessageBoxW(
				NULL,
				g_alpszMessageBoxTexts[5],
				g_alpszTitles[1],
				MB_OK | MB_ICONERROR);

		while(!(stWaveHeader.dwFlags & WHDR_DONE))
			Sleep(1000);
		
		waveOutPause(hWaveDev);

		waveOutUnprepareHeader(
			hWaveDev,
			&stWaveHeader,
			sizeof(stWaveHeader)
			);

		waveOutClose(hWaveDev);

		if (HeapFree(
			GetProcessHeap(), 
			0, 
			stWaveHeader.lpData) == FALSE)
			 {
			MessageBoxW(
				NULL, 
				g_alpszMessageBoxTexts[8], 
				g_alpszTitles[12], 
				MB_OK);
		}
}
// I'll be dynamically allocating memory for audio buffers, since stack size isn't sufficient for it 
VOID WINAPI fnNoise1(VOID) {
	
	LPSTR lpBuff = (LPSTR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 3000000);

	if (lpBuff == NULL) {
			return;
			}

	for(INT iT = 0; iT<3000000; iT++) 
		lpBuff[iT] = (FLOAT)SineWaveMacro(1, 44100 / 48000000, iT, 0) * iT / 60;

	fnPCMplayer(
		lpBuff, 
		39000, 
		3000000
		);


	if (HeapFree(
			GetProcessHeap(), 
			0, 
			lpBuff) == FALSE)
			 {
			MessageBoxW(
				NULL, 
				g_alpszMessageBoxTexts[8], 
				g_alpszTitles[12], 
				MB_OK);
		}
}

VOID WINAPI fnNoise2(VOID) {
	
	LPSTR lpBuff = (LPSTR) HeapAlloc(
		GetProcessHeap(), 
		HEAP_ZERO_MEMORY, 
		3000000
		);

	if (lpBuff == NULL) {
			return;
			}
				
	for(INT iT = 0; iT<3000000; iT++) 
		lpBuff[iT] = (FLOAT)SineWaveMacro(4, 44, iT, 0) * 80000 / 9.2f + iT;

	fnPCMplayer(
		lpBuff, 
		39000, 
		3000000
		);

	if (HeapFree(
			GetProcessHeap(), 
			0, 
			lpBuff) == FALSE)
			 {
			MessageBoxW(
				NULL, 
				g_alpszMessageBoxTexts[8], 
				g_alpszTitles[12], 
				MB_OK);
		}
} 

VOID WINAPI fnSystemSounds(VOID) {
	while(g_iGDIPayloadCount != 7)
		PlaySoundW(
			g_alpszSystemSounds[rand()%2], 
			NULL, 
			SND_SYNC); 
			
	ExitThread(EXIT_SUCCESS);
}
