#ifndef _ASSERV_ASSERV_H_
#define _ASSERV_ASSERV_H_

#include "odo.h"

//arbitrairement
#define TICS_BY_METERD 24950
//#define TICS_BY_METERG 24500
//#define TICS_BY_METERG TICS_BY_METERD
#define METER_BY_TICD (1.0/TICS_BY_METERD)
//#define METER_BY_TICG 0.00004
#define METER_BY_TICG METER_BY_TICD
#define LARGEUR_ROBOT 0.291       // 29.1cm entre encodeurs droite et gauche

#define CONSIGNE_MAX 800
#define CONSIGNE_MIN 120
#define CONSIGNE_NULLE 30

#endif
