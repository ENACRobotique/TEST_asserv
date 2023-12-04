#pragma once
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <math.h>
#include "high_gain_filter.h"
#include "PID.h"

#define SPEED_MAX_ 0.25
#define ACC_ 0.06




class motor{

    private :
        
        PID pid;
        double pos_cons;
        double speed_cons;
        //double error;
        //double epsilon = 0.001;
        double _dt;

    public :

        motor();


        // double get_speed_cons();
        // double get_pos_cons();
        void set_cons(double Pc,double Vc);
        void update();
        
        
};
