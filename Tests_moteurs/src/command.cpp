#include "command.h"



void Command :: set_error(){
    
    //if (this->error > ERROR_MAX_){
    //this->error = 0;
    //}else{

    this->error = this->pos_cons - this->pos;
    //};

};


void Command ::set_acc(){
    this->acc = (2* SPEED_MAX_ - (pos_target - 0.90*pos_target))/2 ;  //on considère un arrêt total en 2s
};


void Command::set_pos_target(double set_pos_target){
    this->pos_target = set_pos_target;
};

void Command::set_pos(double set_pos){
    this->pos = set_pos;
};

void Command::set_speed(double set_speed){
    this->speed = set_speed;
};

void Command::set_cmd(int cmd){
    this->cmd = cmd;
};


void Command::set_cons(){
    /*
determine la trajectoire souhaitée et la consigne de vitesse correspondante selon la phase de déplacement du robot
    */
   
    switch(this->state){

        case acceleration:{
            this->pos_cons =  0.5 * ACC_  * pow(millis()/1000,2);
            this->speed_cons = ACC_*millis()/1000;
            this->bridge_pos = this->pos;
            break;
        };
        case vitesse_cte:{
            this->pos_cons = SPEED_MAX_*millis()/1000;
            this->speed_cons = SPEED_MAX_;
            this->bridge_pos = this->pos;
            break;
        };

        case decceleration:{
            this->pos_cons = 0.5 * (this->acc) * pow(millis()/1000,2)  + SPEED_MAX_ *millis()/1000 + this->bridge_pos  ; 
            this->speed_cons = SPEED_MAX_ - this->acc*millis()/1000;
            this->pos;
            break;
        };
    };

};


void Command::set_state(){
    
    if (this->pos >= 0.90* this->pos_target){
        this->state = decceleration;}

    
    else if(this->speed_cons>=0.9*SPEED_MAX_){
        this->state = vitesse_cte;

    }
    else{

        this->state = acceleration;
    };
}


double Command::get_error(){
    return this->error;
};


double Command::get_speed_cons(){
    return this->speed_cons;
};


double Command::get_speed(){
    return this->speed;
};

double Command::get_pos_cons(){
    return this->pos_cons;
};

double Command::get_pos(){
    return this->pos;
};

int Command::get_cmd(){
    return this->cmd;
};