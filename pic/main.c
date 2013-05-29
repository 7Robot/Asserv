/*
* Asserv dsPIC33F
* Compiler : Microchip xC16
* µC : 33FJ64MC804
* Avril 2013
*    ____________      _           _
*   |___  /| ___ \    | |         | |
*      / / | |_/ /___ | |__   ___ | |_
*     / /  |    // _ \| '_ \ / _ \| __|
*    / /   | |\ \ (_) | |_) | (_) | |_
*   /_/    |_| \_\___/|____/ \___/'\__|
*                 7robot.fr
*/

#include "header.h"  /* Function / Parameters                           */
#include "asserv/libasserv.h"
#include "atp-asserv.h"

/******************************************************************************/
/* Configuartion                                                              */
/******************************************************************************/

// Select Oscillator and switching.
_FOSCSEL(FNOSC_FRCPLL & IESO_OFF);
// Select clockBuff_adc.
_FOSC(POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSDCMD);
// Watchdog Timer.
_FWDT(FWDTEN_OFF);
// Select debug channel.
_FICD(ICS_PGD1 & JTAGEN_OFF);


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    // Initialize IO ports and peripherals.
    ConfigureOscillator();
    InitApp();
    InitAdc();
    AtpInit();
//    motion_init(24700, 0.29, SendDone);

    SendBoardId();


    while(1) {
        //SendBoardId();

//        if (odoBroadcast) {
//            OnGetPos();
        //}

        __delay_ms(1000);

    }
}

