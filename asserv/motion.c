#include <math.h>
#include <stdio.h>

#include "libasserv_priv.h"

#define PI 3.141592653589793

// Mode de génération des rampes
#define M_MANUAL    0 // pas de génération
#define M_POS       1 // rampe de position
#define M_SPEED     2 // rampe de vitesse

// état des systèmes (position, vitesse)
static volatile State deltaState;
static volatile State alphaState;

// commandes actuelles
static volatile Order deltaOrder;
static volatile Order alphaOrder;

// consignes finales
static volatile Order deltaFinalOrder;
static volatile Order alphaFinalOrder;

// asserv
static volatile Asserv deltaAsserv;
static volatile Asserv alphaAsserv;

// génération des rampes
static volatile int deltaMode = M_MANUAL;
static volatile int alphaMode = M_MANUAL;

// seuils de détection d’atteinte de la consigne
static volatile float epsDist = DEFAULT_EPSILON_DIST;
static volatile float epsSpeed = DEFAULT_EPSILON_SPEED;
static volatile float epsTheta = DEFAULT_EPSILON_THETA;
static volatile float epsOmega = DEFAULT_EPSILON_OMEGA;

// accélération, décélération et vitesse maximum
static volatile float vDistMax, aDistMax, dDistMax;
static volatile float vRotMax, aRotMax, dRotMax;
static volatile float vDistMaxDefault = DEFAULT_DIST_SPEED_MAX;
static volatile float aDistMaxDefault = DEFAULT_DIST_ACC_MAX;
static volatile float dDistMaxDefault = DEFAULT_DIST_DEC_MAX;
static volatile float vRotMaxDefault = DEFAULT_ROT_SPEED_MAX;
static volatile float aRotMaxDefault = DEFAULT_ROT_ACC_MAX;
static volatile float dRotMaxDefault = DEFAULT_ROT_DEC_MAX;

static void(*done)(void); // callback

void motion_init(void(*_done)(void)) {
    odo_init();
    done = _done;
    asserv_init(&deltaAsserv,
            (PidCoefs)DEFAULT_DELTA_POS_COEFS,
            (PidCoefs)DEFAULT_DELTA_SPEED_COEFS,
            &deltaOrder);
    asserv_init(&alphaAsserv,
            (PidCoefs)DEFAULT_ALPHA_POS_COEFS,
            (PidCoefs)DEFAULT_ALPHA_SPEED_COEFS,
            &alphaOrder);
    deltaState.x = 0; alphaState.x = 0;
}

void motion_step(float period, int ticsLeft, int ticsRight, int *cmdLeft, int *cmdRight) {
    int ret;
    float dist, rot;
    float cmdDelta, cmdAlpha;

    odo_step(ticsLeft, ticsRight, &dist, &rot);

    deltaState.x += dist;
    deltaState.v = dist / period;
    alphaState.x += rot;
    alphaState.v = rot / period;

    switch (deltaMode) {
        case M_POS:
            ret = ramp_dist(period,
                    &(deltaOrder.x), &(deltaOrder.v), &(deltaOrder.a),
                    deltaFinalOrder.x, deltaFinalOrder.v, vDistMax, aDistMax);
            if (ret) {
                deltaMode = M_MANUAL;
                asserv_off(&(deltaAsserv));
                done();
            }
            break;
        case M_SPEED:
            ramp_speed(period,
                    &(deltaOrder.x), &(deltaOrder.v),
                    deltaFinalOrder.v,
                    aDistMax, vDistMax, dDistMax);
            break;
    }
    switch (alphaMode) {
        case M_POS:
            ret = ramp_dist(period,
                    &(alphaOrder.x), &(alphaOrder.v), &(alphaOrder.a),
                    alphaFinalOrder.x, alphaFinalOrder.v, vRotMax, aRotMax);
            if (ret) {
                alphaMode = M_MANUAL;
                asserv_off(&(alphaAsserv));
                done();
            }
            break;
        case M_SPEED:
            ramp_speed(period,
                    &(alphaOrder.x), &(alphaOrder.v),
                    alphaFinalOrder.v,
                    aRotMax, vRotMax, dRotMax);
            break;
    }

    /*if (mode == M_END || mode == M_DIST_FREE || mode == M_ROT_FREE) {
        if (asserv_done(epsDist, epsSpeed, epsTheta, epsOmega)) {
            asserv_off();
            mode = M_MANUAL;
            if (done) done();
        }
    }*/

    cmdDelta = asserv_step(&deltaAsserv, period, deltaState);
    cmdAlpha = asserv_step(&alphaAsserv, period, alphaState);

    *cmdLeft = (int)(cmdDelta - 2 * cmdAlpha);
    *cmdRight = (int)(cmdDelta + 2 * cmdAlpha);
}

/*void OnStep(unsigned long int period, unsigned long int ticsG, unsigned long int ticsD, unsigned long int consignG, unsigned long int consignD) {
    motion_step(period, ticsG, ticsD, (int)consignG, (int)consignD);
}*/

void motion_dist(float dist, float v, float a) {
    vDistMax = (v>0)?v:vDistMaxDefault;
    aDistMax = (a>0)?a:aDistMaxDefault;
    // delta
    deltaMode = M_POS;
    deltaState.x = 0;
    deltaOrder.x = 0;
    deltaFinalOrder.x = dist;
    deltaFinalOrder.v = 0;
    asserv_set_pos_mode(&deltaAsserv);
    // alpha
    alphaMode = M_MANUAL;
    alphaState.x = 0;
    alphaState.v = 0;
    alphaOrder.x = 0;
    asserv_set_pos_mode(&alphaAsserv);
}

void OnDist(float dist, float vMax, float aMax) { motion_dist(dist, vMax, aMax); }

void motion_dist_free(float dist) {
    // delta
    deltaMode = M_MANUAL;
    deltaState.x = 0;
    deltaState.v = 0;
    deltaOrder.x = dist;
    asserv_set_pos_mode(&deltaAsserv);
    // alpha
    alphaMode = M_MANUAL;
    alphaState.x = 0;
    alphaState.v = 0;
    alphaOrder.x = 0;
    asserv_set_pos_mode(&alphaAsserv);
}

void OnDistFree(float dist) { motion_dist_free(dist); }

void motion_rot(float rot, float v, float a) {
    vRotMax = (v>0)?v:vRotMaxDefault;
    aRotMax = (a>0)?a:aRotMaxDefault;
    // delta
    deltaMode = M_MANUAL;
    deltaState.x = 0;
    deltaState.v = 0;
    deltaOrder.x = 0;
    asserv_set_pos_mode(&deltaAsserv);
    // alpha
    alphaMode = M_POS;
    alphaState.x = 0;
    alphaOrder.x = 0;
    alphaFinalOrder.x = rot;
    alphaFinalOrder.v = 0;
    asserv_set_pos_mode(&alphaAsserv);
}

void OnRot(float rot, float vMax, float aMax) { motion_rot(rot, vMax, aMax); }

void motion_rot_free(float rot) {
    // delta
    deltaMode = M_MANUAL;
    deltaState.x = 0;
    deltaState.v = 0;
    deltaOrder.x = 0;
    asserv_set_pos_mode(&deltaAsserv);
    // alpha
    alphaMode = M_MANUAL;
    alphaState.x = 0;
    alphaState.v = 0;
    alphaOrder.x = rot;
    asserv_set_pos_mode(&alphaAsserv);
}

void OnRotFree(float rot) { motion_rot_free(rot); }

void motion_dist_rot(float dist, float rot, float vDist, float aDist, float vRot, float aRot) {
    // delta
    vDistMax = (vDist>0)?vDist:vDistMaxDefault;
    aDistMax = (aDist>0)?aDist:aDistMaxDefault;
    deltaMode = M_POS;
    deltaState.x = 0;
    deltaOrder.x = 0;
    deltaFinalOrder.x = dist;
    deltaFinalOrder.v = 0;
    asserv_set_pos_mode(&deltaAsserv);
    // alpha
    vRotMax = (vRot>0)?vRot:vRotMaxDefault;
    aRotMax = (aRot>0)?aRot:aRotMaxDefault;
    alphaMode = M_POS;
    alphaState.x = 0;
    alphaOrder.x = 0;
    alphaFinalOrder.x = rot;
    alphaFinalOrder.v = 0;
    asserv_set_pos_mode(&alphaAsserv);
}

void OnDistRot(float dist, float rot, float vDistMax, float aDistMax, float vRotMax, float aRotMax) {
    motion_dist_rot(dist, rot, vDistMax, aDistMax, vRotMax, aRotMax);
}

void motion_reach_x(float x, float v, float a) {
    float dist = (x - odo_get_x()) / cos(odo_get_theta());
    motion_dist(dist, v, a);
}

void motion_reach_y(float y, float v, float a) {
    float dist = (y - odo_get_y()) / sin(odo_get_theta());
    motion_dist(dist, v, a);
}

void motion_reach_theta(float theta, float v, float a) {
    float rot = theta - odo_get_theta();
    while (rot > PI) rot -= 2*PI;
    while (rot < -PI) rot += 2*PI;
    motion_rot(rot, v, a);
}

void motion_speed(float v, float a, float d) {
    aDistMax = (a>0)?a:aDistMaxDefault;
    dDistMax = (d>0)?d:dDistMaxDefault;
    vDistMax = (v<0)?-v:v;
    // delta
    deltaMode = M_SPEED;
    deltaFinalOrder.v = v;
    asserv_set_speed_mode(&deltaAsserv);
    // alpha
    alphaMode = M_MANUAL;
    alphaState.x = 0;
    alphaState.v = 0;
    alphaOrder.x = 0;
    asserv_set_pos_mode(&alphaAsserv);
}

void OnSpeed(float speed, float aMax, float dMax) { motion_speed(speed, aMax, dMax); }

void motion_speed_free(float speed) {
    // delta
    deltaMode = M_MANUAL;
    deltaState.x = 0;
    deltaState.v = 0;
    deltaOrder.v = speed;
    asserv_set_speed_mode(&deltaAsserv);
    // alpha
    alphaMode = M_MANUAL;
    alphaState.x = 0;
    alphaState.v = 0;
    alphaOrder.x = 0;
    asserv_set_pos_mode(&alphaAsserv);
}

void OnSpeedFree(float speed) { motion_speed_free(speed); }

void motion_omega(float omega, float a, float d) {
    aRotMax = (a>0)?a:aRotMaxDefault;
    dRotMax = (d>0)?d:dRotMaxDefault;
    vRotMax = (omega<0)?-omega:omega;
    // delta
    deltaMode = M_MANUAL;
    deltaState.x = 0;
    deltaState.v = 0;
    deltaOrder.x = 0;
    asserv_set_pos_mode(&deltaAsserv);
    // alpha
    alphaMode = M_SPEED;
    alphaFinalOrder.v = omega;
    asserv_set_speed_mode(&alphaAsserv);
}

void OnOmega(float omega, float aMax, float dMax) { motion_omega(omega, aMax, dMax); }

void motion_omega_free(float omega) {
    // delta
    deltaMode = M_MANUAL;
    deltaState.x = 0;
    deltaState.v = 0;
    deltaOrder.x = 0;
    asserv_set_pos_mode(&deltaAsserv);
    // alpha
    alphaMode = M_MANUAL;
    alphaState.x = 0;
    alphaState.v = 0;
    alphaOrder.v = omega;
    asserv_set_speed_mode(&alphaAsserv);
}

void motion_speed_omega(float speed, float omega, float aDist, float dDist, float aRot, float dRot) {
    // delta
    aDistMax = (aDist>0)?aDist:aDistMaxDefault;
    dDistMax = (dDist>0)?dDist:dDistMaxDefault;
    vDistMax = (speed<0)?-speed:speed;
    deltaMode = M_SPEED;
    deltaFinalOrder.v = speed;
    asserv_set_speed_mode(&deltaAsserv);
    // alpha
    aRotMax = (aRot>0)?aRot:aRotMaxDefault;
    dRotMax = (dRot>0)?dRot:dRotMaxDefault;
    vRotMax = (omega<0)?-omega:omega;
    alphaMode = M_SPEED;
    alphaFinalOrder.v = omega;
    asserv_set_speed_mode(&alphaAsserv);
}

void OnSpeedOmega(float speed, float omega, float aDistMax, float dDistMax, float aRotMax, float dRotMax) {
    motion_speed_omega(speed, omega, aDistMax, dDistMax, aRotMax, dRotMax);
}

//void motion_pause(float aDistMax, float dDistMax) {
//    consigne.rot = 0;
//    asserv_set_consigne(&consigne);
//    motion_rampe_init(0, dt, speed.v, 0, fabs(speed.v), aDistMax, dDistMax, 1);
//    set_mode(M_DIST);
//}

void motion_stop() {
    deltaMode = M_MANUAL;
    deltaState.x = 0;
    deltaState.v = 0;
    asserv_off(&deltaAsserv);
    alphaMode = M_MANUAL;
    alphaState.x = 0;
    alphaState.v = 0;
    asserv_off(&alphaAsserv);
    done();
}

void OnStop() { motion_stop(); }

void motion_set_epsilons(float Ed, float Es, float Et, float Eo) {
    epsDist = Ed;
    epsSpeed = Es;
    epsTheta = Et;
    epsOmega = Eo;
}

void OnSetEpsilons(float dist, float speed, float theta, float omega) {
    motion_set_epsilons(dist, speed, theta, omega);
}
