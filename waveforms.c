#include "waveforms.h"
//Sine wave

FLOAT g_afWave[48000];
INT g_iSample = 0;

VOID WINAPI fnSineWave(VOID) {
FLOAT fRad = 3.6 * M_PI * 44100 / 480000000;
ZeroMemory(&g_afWave, 48000);

      while (TRUE) {
      g_afWave[g_iSample] = sin(g_iSample * fRad);
      g_iSample = (g_iSample + 1) % 48000;
      }


}
   