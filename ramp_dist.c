#include <math.h>
#include <stdio.h>

int ramp_dist(float dt,
        volatile float *pos, volatile float *speed, volatile float *acc,
        float posEnd, float speedEnd,
        float speedMax, float accMax,
        int noExceeding)
{
    if (speedMax == 0 || accMax == 0) return -1;
    if (speedMax < 0) speedMax = -speedMax;
    if (accMax < 0) accMax = -accMax;

    if (speedEnd > speedMax) speedEnd = speedMax;
    else if (speedEnd < -speedMax) speedEnd = -speedMax;

    int testPos = posEnd > *pos;
    int testSpeed = speedEnd > *speed;
    int atSpeed = 1;

    float dist = posEnd - *pos;
    if (dist < 0 && *speed > 0) {
        *acc = -accMax;
    } else if (dist > 0 && *speed < 0) {
        *acc = accMax;
    } else if (dist < 0 && *speed <= 0 && speedEnd > 0) {
        if (*speed < -speedMax) {
            *acc = accMax;
            atSpeed = -1;
        } else {
            float t1 = *speed / (-accMax);
            float d1 = - 0.5f * (-accMax) * t1 * t1 + *speed * t1;
            float t2 = speedEnd / accMax;
            float d2 = - 0.5f * accMax * t2 * t2;
            float epsilon = *speed * dt / 2.0f;
            if (d1 >= dist + d2 - epsilon) {
                if (*speed > -speedMax) {
                    *acc = -accMax;
                } else {
                    *acc = 0;
                }
            } else if (d1 >= dist + d2 - 2*epsilon) {
                *acc = 0;
            } else {
                *acc = accMax;
            }
        }
        atSpeed = -1;
    } else if (dist > 0 && *speed >= 0 && speedEnd < 0) {
        if (*speed > speedMax) {
            *acc = -accMax;
            atSpeed = -1;
        } else {
            float t1 = *speed / accMax;
            float d1 = - 0.5f * accMax * t1 * t1 + *speed * t1;
            float t2 = speedEnd / (-accMax);
            float d2 = - 0.5f * (-accMax) * t2 * t2;
            float epsilon = *speed * dt / 2.0f;
            if (d1 <= dist + d2 - epsilon) {
                if (*speed < speedMax) {
                    *acc = accMax;
                } else {
                    *acc = 0;
                }
            } else if (d1 <= dist + d2 - 2*epsilon) {
                *acc = 0;
            } else {
                *acc = -accMax;
            }
        }
        atSpeed = -1;
    } else if (dist < 0 && *speed <= 0 && speedEnd <= 0) {
        if (*speed < -speedMax) {
            *acc = accMax;
            atSpeed = -1;
        } else if (*speed > speedEnd) {
            *acc = -accMax;
        } else {
            float t = (*speed - speedEnd) / (-accMax);
            float d = - 0.5f * (-accMax) * t * t + *speed * t;
            float epsilon = *speed * dt / 2.0f;
            if (d <= dist - epsilon) {
                *acc = accMax;
            } else if (d <= dist - 2*epsilon) {
                *acc = 0;
            } else {
                if (*speed > -speedMax) {
                    *acc = -accMax;
                } else {
                    *acc = 0;
                }
            }
        }
    } else if (dist > 0 && *speed >= 0 && speedEnd >= 0) {
        if (*speed > speedMax) {
            *acc = -accMax;
            atSpeed = -1;
        } else if (*speed < speedEnd) {
            *acc = accMax;
        } else {
            float t = (*speed - speedEnd) / accMax;
            float d = - 0.5f * accMax * t * t + *speed * t;
            float epsilon = *speed * dt / 2.0f;
            if (d >= dist - epsilon) {
                *acc = -accMax;
            } else if (d >= dist - 2*epsilon) {
                *acc = 0;
            } else {
                if (*speed < speedMax) {
                    *acc = accMax;
                } else {
                    *acc = 0;
                }
            }
        }
    } else {
        *acc = 0;
    }

    *speed += *acc * dt;
    *pos += *speed * dt;
    int atPos = testPos ^ (posEnd >= *pos);
    if (atSpeed < 0) {
        atSpeed = testSpeed ^ (speedEnd >= *speed);
    }

    if (noExceeding) {
        return atPos;
    } else {
        return atPos && atSpeed;
    }
}
