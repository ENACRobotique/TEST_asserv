#include "motor.h"
#include <math.h>

constexpr float DT = 0.025; // 25ms

constexpr float POS_ALPHA0 = 2.4f;
constexpr float POS_ALPHA1 = 2.08f;
constexpr float POS_ALPHA2 = 0.64f;
constexpr float POS_EPS = 0.04f; // 0.02

float alphas[] = {POS_ALPHA0, POS_ALPHA1, POS_ALPHA2};

double clamp(double lo, double val, double hi) {
    return min(max(lo, val), hi);
}

motor::motor() {
    high_gain_filter_init(&pos_filter, alphas, POS_EPS, 1000 );
}


void motor::set_cons(double Pc,double Vc){
    double pcmin = get_pos() + get_speed()*_dt - (ACC_*_dt*_dt)/2;
    double pcmax = get_pos() + get_speed()*_dt + (ACC_*_dt*_dt)/2;
    double vcmin = get_speed() - ACC_*_dt;
    double vcmax = get_speed() + ACC_*_dt;

    pos_cons = clamp(pcmin, Pc, pcmax);
    speed_cons = clamp(vcmin, Vc, vcmax);
};

void motor::update() {
    double pos_error = pos_cons - get_pos();
    double speed_error = speed_cons - get_speed();
    double cmd = pid.update_cmd(pos_error, speed_error, _dt);
    
    cmd = clamp(-255, cmd, 255);

    // command motor
    
}