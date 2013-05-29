#include "odo.h"
#include "atp.h"
#include "asserv/libasserv.h"

bool odoBroadcast = false;
int odoDelay = 400; // en millisecondes

void OnOdoBroadcastSetDelay(unsigned int new_delay) {
    if (new_delay > 0) {
        odoDelay = new_delay;
    } else {
        SendError();
    }   
}

void OnOdoBroadcastOn() { odoBroadcast = true; }
void OnOdoBroadcastOff() { odoBroadcast = false; }

void OnGetPos() {
    SendPos(odo_get_x(), odo_get_y(), odo_get_theta());
}
void OnSetOdoTheta(float theta) {
    odo_set_theta(theta);
}
void OnSetOdoX(float x) {
    odo_set_x(x);
}
void OnSetOdoXTheta(float x, float theta) {
    odo_set_x(x);
    odo_set_theta(theta);
}
void OnSetOdoXY(float x, float y) {
    odo_set_x(x);
    odo_set_y(y);
}
void OnSetOdoXYTheta(float x, float y, float theta) {
    odo_set_x(x);
    odo_set_y(y);
    odo_set_theta(theta);
}
void OnSetOdoY(float y) {
    odo_set_y(y);
}
void OnSetOdoYTheta(float y, float theta) {
    odo_set_y(y);
    odo_set_theta(theta);
}

void OnSetOdoTicByMeter(int tic_by_meter) {
    odo_set_tic_by_meter(tic_by_meter);
}

void OnSetOdoSpacing(float spacing) {
    odo_set_spacing(spacing);
}