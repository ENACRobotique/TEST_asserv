#ifndef COMMAND_H
#define COMMAND_H

#pragma once
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <math.h>

#define SPEED_MAX_ 0.25
#define ACC_ 0.06
#define ERROR_MAX_  0.60

typedef enum{
    acceleration =1,
    vitesse_cte =2,
    decceleration=3,
    arret=4
}etat_cmd;

class Command{

    private :
        int cmd;
        double pos_target;
        double pos = 0;
        double pos_cons;
        double bridge_pos; // position où le robot deccelere
        double speed;
        double speed_target;
        double speed_cons;
        double error;
        double acc;
        double time_previous_state;
        double time_first_state;
        double pos_end_acc;
        double epsilon = 0.001;
        double stop_time = 3.;
        etat_cmd state = acceleration;

    public :

        Command(double target);

        //int get_cmd();
        double get_error();
        double get_speed_cons();
        double get_speed();
        double get_pos_cons();
        double get_acc(){return this->acc;};

        double get_pos();

        int get_cmd();

        double get_pos_target();

        void set_cons();
        void set_cmd(int cmd);
        void set_error();
        void set_acc();
        
        void set_pos(double set_pos);
        void set_speed(double set_speed);
        
        //void set_pos_target(double set_target);
        void set_state();
        
        etat_cmd get_state(){return this->state;};
};

#endif