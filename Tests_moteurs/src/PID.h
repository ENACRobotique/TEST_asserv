#ifndef PID_H
#define PID_H

#pragma once
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <math.h>

class PID{

    private:
        double kp=2.7;
        double ki=6.1;
        double kd=0.012;
        double min;
        double max;
        double integral=0;
        double derivative;
        int cmd;

    public :

        int update_cmd(double error);
        void set_integral(double error,double freq);
        void set_derivative(double speed_cons, double speed);
        double get_kp();
        double get_ki();
        double get_kd();
        double get_integral();
        double get_derivative();

        

};

#endif