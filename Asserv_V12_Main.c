/*
*    ____________      _           _
*   |___  /| ___ \    | |         | |
*      / / | |_/ /___ | |__   ___ | |_
*     / /  |    // _ \| '_ \ / _ \| __|
*    / /   | |\ \ (_) | |_) | (_) | |_
*   /_/    |_| \_\___/|____/ \___/'\__|
*                 7robot.fr
*/

#include <p33Fxxxx.h>      /* Includes device header file                     */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include "Asserv_V12_Header.h"    /* Function / Parameters                           */
#include <libpic30.h>
#include <uart.h>
#include "atp-asserv.h"
#include "atp.h"
#include <adc.h>
#include "libasserv.h"

_FOSCSEL(FNOSC_FRCPLL & IESO_OFF);
_FOSC(POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSDCMD);
_FWDT(FWDTEN_OFF);
_FICD(ICS_PGD1 & JTAGEN_OFF);

int16_t main(void) {
    AD1PCFGL = 0x1FF;
    ConfigureOscillator();
    InitApp();
    AtpInit();
    InitPWM();

    SendBoardId();
    __delay_ms(2000);
    SendBoardId();

    motion_dist(1,1,1);
    
    while(1) __delay_ms(1000);
}
