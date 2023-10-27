#include "PID.h"
#include <Arduino.h>

double PID :: get_kp(){
    return this->kp;
};

double PID :: get_kd(){
    return this->kd;
};

double PID :: get_ki(){
    return this->ki;
};

double PID :: get_integral(){
    return this->integral;
};

double PID :: get_derivative(){
    return this->derivative;
};

int PID ::update_cmd(double error){

    return  get_kp() * error + get_ki() * get_integral() + get_kd() *get_derivative() ;

};

void PID ::set_integral(double error, double freq){ // la frequence est le taux de rafraichissement. Par exemple, si on appel la fonction toutes les 25ms, freq = 0.025
    double integral_max = 5;
    double new_integral = this->integral +error*freq;
    this->integral = new_integral;
};

void PID ::set_derivative(double speed_cons, double speed){ 
    this->derivative = speed_cons - speed;
};