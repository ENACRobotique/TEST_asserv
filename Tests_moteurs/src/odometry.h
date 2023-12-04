#pragma once

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <math.h>
#include <Eigen.h>
#include "high_gain_filter.h"

#define MOTORS_NB 3

class Odometry{

    private :
        high_gain_filter pos_filter[MOTORS_NB];

        Eigen::Vector3d pos;
        Eigen::Vector3d speed;

    public :

        Eigen::Vector3d get_pos();
        Eigen::Vector3d get_speed();
        void update();
        double get_motor_pos(int motor_no);
        double get_motor_speed(int motor_no);

        
        
};
