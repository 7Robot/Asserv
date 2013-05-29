#include "asserv/libasserv.h"

void SendError(); // from atp-asserv.h

float vDistMax = 0.5, aDistMax = 0.5, dDistMax = 0.5;
float vRotMax = 3, aRotMax = 3, dRotMax = 3;

void OnDist(float dist) {
    motion_dist(dist, vDistMax, aDistMax);
}
void OnDistRot(float dist, float rot) {
    motion_dist_rot(dist, rot, vDistMax, aDistMax, vRotMax, aRotMax);
}
void OnDistSansRampe(float dist) {
    motion_dist_free(dist);
}
void OnSpeed(float speed) {
    motion_speed(speed, aDistMax, dDistMax);
}
void OnSpeedSansRampe(float speed) {
    motion_speed_free(speed);
}
void OnOmega(float omega) {
    motion_omega(omega, aRotMax, dRotMax);
}
void OnOmegaSansRampe(float omega) {
    motion_omega_free(omega);
}
void OnPause() {
    // TODO
}
void OnReachTheta(float theta) {
    motion_reach_theta(theta, vRotMax, aRotMax);
}
void OnReachX(float x) {
    motion_reach_x(x, vDistMax, aDistMax);
}
void OnReachY(float y) {
    motion_reach_y(y, vDistMax, aDistMax);
}
void OnRot(float rot) {
    motion_rot(rot, vRotMax, aRotMax);
}
void OnRotSansRampe(float rot) {
    motion_rot_free(rot);
}

/********* R�glages des coefs ******************/
void OnSetDeltaCoefs(float p, float i, float d) { // pid lin�aire
    asserv_set_delta_coefs(p, i, d);
}
void OnSetAlphaCoefs(float p, float i, float d) { // pid rotation
    asserv_set_alpha_coefs(p, i, d);
}
// condition d?acceptation pour s?arr�ter
void OnSetEpsilons(float Ed, float Ev, float Et, float Eo) {
    motion_set_epsilons(Ed, Ev, Et, Eo);
}
// Limites de vitesses et d?acc�l�ration pour la g�n�ration des rampes
void OnSetDistLimits(float vMax, float aMax, float dMax) { // lin�aire
    vDistMax = vMax; aDistMax = aMax; dDistMax = dMax;
}
void OnSetRotLimits(float vMax, float aMax, float dMax) { // rotation
    vRotMax = vMax; aRotMax = aMax; dRotMax = dMax;
}

// D�sactivation de l?asserv
void OnStop() {
    motion_stop();
}
