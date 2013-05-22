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
#include <qei.h>           /* Includes qei functions                          */
#include <timer.h>         /* Include timer fonctions                         */
#include <uart.h>
#include "Asserv_V12_Header.h"
#include <libpic30.h>
#include "atp-asserv.h"
#include "libasserv.h"

void ConfigureOscillator() {
    PLLFBDbits.PLLDIV = 41; // M=43
    CLKDIVbits.PLLPOST = 0; // N1=2
    CLKDIVbits.PLLPRE  = 0; // N2=2
}

void InitApp() {
    _TRISB4 = 0;
    _TRISA9 = 0;
    _TRISA8 = 0;
    _NSTDIS = 0;
    _TRISB2 = 0;
    _TRISB3 = 0;
    _TRISB4 = 0;
    _TRISA4 = 0;
    
    OpenTimer2(T2_ON & T2_GATE_OFF & T2_PS_1_256 & T2_32BIT_MODE_OFF & T2_SOURCE_INT, 1500);
    ConfigIntTimer2(T2_INT_PRIOR_3 & T2_INT_ON); //Interruption ON et priorite 3
    
    OpenQEI1(QEI_DIR_SEL_QEB & QEI_INT_CLK & QEI_INDEX_RESET_DISABLE & QEI_CLK_PRESCALE_1 & QEI_NORMAL_IO & QEI_MODE_x4_MATCH & QEI_UP_COUNT,0);
    OpenQEI2(QEI_DIR_SEL_QEB & QEI_INT_CLK & QEI_INDEX_RESET_DISABLE & QEI_CLK_PRESCALE_1 & QEI_NORMAL_IO & QEI_MODE_x4_MATCH & QEI_UP_COUNT,0);

    _QEA1R = 9;     //Module QEI 1 phase A sur RP9, RB9
    _QEB1R = 22;    //Module QEI 1 phase B sur RP22, RC6
    _QEA2R = 23;    //Module QEI 2 phase A sur RP23, RC7
    _QEB2R = 24;    //Module QEI 2 phase B sur RP24, RC8

    POS1CNT = 0;    // 0 tic pour l'instant
    POS2CNT = 0;

}

void InitPWM() {
    PTCONbits.PTEN   = 0;   // Timer Enable bit:            DISABLE MCPWM
    PWMCON1bits.PEN1H = 1;  // PWM1H (pin 37) is enabled for PWM output
    PWMCON1bits.PEN2H = 1;  // PWM2H (pin 35) is enabled for PWM output
    PTCONbits.PTCKPS = 1;   // Input Clock Prescale bits:   1:4
    PTCONbits.PTOPS  = 0;   // Output Clock Postscale bits: 1:1
    PTCONbits.PTSIDL = 1;    // Stop in Idle Mode:           YES
    PTCONbits.PTMOD  = 0;   // Mode Select bits:            Free Running Mode
    PTCONbits.PTEN   = 1;   // Timer Enable bit:             ENABLE MCPWM

    /**** PTPER: PWM Time Base Period Register ****/
    PTPERbits.PTPER = 500; // Period Value bits

    P1DC1 = 0;
    P1DC2 = 0;          // rapport cycliques nuls pour les moteurs

    _TRISC3 = 0;    //DIRA1 sur RC3 en sortie
    _TRISC4 = 0;    //DIRB1 sur RC4 en sortie
    _TRISA3 = 0;    //DIRA2 sur RA3 en sortie
    _TRISA4 = 0;    //DIRB2 sur RA4 en sortie
}

volatile int16_t ticd = 0, old_ticd = 0, diffd = 0;
volatile int16_t ticg = 0, old_ticg = 0, diffg = 0;
volatile int32_t compteur_ticd = 0;
volatile int32_t compteur_ticg = 0;

void __attribute__((interrupt,auto_psv)) _T2Interrupt(void) {
    int cmdLeft = 0, cmdRight = 0;
    // On lit l'encodeur droit (qui est en fait le gauche)
    ticd = (uint16_t) POS1CNT;// ReadQEI2();
    diffd = ticd-old_ticd;
    old_ticd = ticd;
    compteur_ticd += diffd;

    // On lit l'encodeur gauche (qui est en fait le droit)
    ticg = (uint16_t) POS2CNT;// ReadQEI1();
    diffg = ticg-old_ticg;
    old_ticg = ticg;
    compteur_ticg += diffg;

    motion_step(0.01, diffg, diffd, &cmdLeft, &cmdRight);
    moteurs(cmdLeft, cmdRight);
}

void moteurs(int g, int d) {
    if (g < 0) {
        g = -g;
        DIRA2 = 0;
        DIRB2 = 1;
    } else {
        DIRA2 = 1;
        DIRB2 = 0;
    }
    if (d < 0) {
        d = -d;
        DIRA1 = 0;
        DIRB1 = 1;
    } else {
        DIRA1 = 1;
        DIRB1 = 0;
    }

    if (g>PWM_MAX) g = PWM_MAX;
    if (d>PWM_MAX) d = PWM_MAX;

    P1DC1 = d;
    P1DC2 = g;
}
