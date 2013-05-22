/*
*    ____________      _           _
*   |___  /| ___ \    | |         | |
*      / / | |_/ /___ | |__   ___ | |_
*     / /  |    // _ \| '_ \ / _ \| __|
*    / /   | |\ \ (_) | |_) | (_) | |_
*   /_/    |_| \_\___/|____/ \___/'\__|
*                 7robot.fr
*/

#include "atp.h"

#define SYS_FREQ        80000000 //7370000L
#define FCY             SYS_FREQ/2

#define led1 _LATB4
#define led2 _LATA8

#define DIRA1 _LATC3
#define DIRB1 _LATC4
#define DIRA2 _LATA3
#define DIRB2 _LATA4

#define PWM_MAX 800

void ConfigureOscillator(); /* Handles clock switching/osc initialization */
void InitApp();             /* I/O and Peripheral Initialization          */
void InitPWM();
void InitAdc();
void moteurs(int,int);