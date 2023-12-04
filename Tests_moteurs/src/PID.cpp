#include "PID.h"
#include <Arduino.h>



double PID ::update_cmd(double error, double speed_error, double period){

    integral = integral +error*period;
    derivative = speed_error;

    return  kp * error + ki * integral + kd *derivative ;
};
