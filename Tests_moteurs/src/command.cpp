#include "command.h"


Command::Command(double target){
    this->pos_target = target;
    this->set_acc();

}

void Command :: set_error(){
    
    //if (this->error > ERROR_MAX_){
    //this->error = 0;
    //}else{

    this->error = this->pos_cons - this->pos;
    //};

};


void Command::set_acc(){
    this->acc = 2*(this->stop_time*SPEED_MAX_ - 0.1*this->pos_target)/pow(this->stop_time,2) ;  //on considère un arrêt total en 2s
};


// void Command::set_pos_target(double set_target){
    // this->pos_target = set_target;
// };

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
            this->pos_cons =  0.5 * ACC_  * pow(millis()/1000.0,2);
            this->speed_cons = min(ACC_*millis()/1000.0,SPEED_MAX_);
            this->bridge_pos = this->pos;

            this->time_previous_state = (millis()/1000.0);
            this->time_first_state=this->time_previous_state;

            if (this->pos >= 0.80* this->pos_target){
                this->state = decceleration;

            } else if (abs(this->pos - this->pos_target) <= this->epsilon){
                this->state = arret;

            }else if(this->speed >=SPEED_MAX_){
                this->state = vitesse_cte;
                this->pos_end_acc= this->pos;
            } ;

            break;
        };

        case vitesse_cte:{
            
            this->pos_cons = min(SPEED_MAX_*((double)millis()/1000.0 - this->time_first_state) + this->pos_end_acc,this->pos_target);
            this->speed_cons = SPEED_MAX_;

            this->time_previous_state = ((double)millis()/1000.0); //on garde en mémoire le temps d'avant le changement d'etat car il faut le reinitialiser pour l'etat suivant
            this->bridge_pos = this->pos_cons;

            
            if (abs(this->pos - this->pos_target) <= this->epsilon){
                this->state = arret;

            }else if (this->pos >= 0.80* this->pos_target){
                this->state = decceleration;
            };

            break;
        };

        case decceleration:{
            this->pos_cons = -0.5 * (this->acc) * pow(((double)millis()/1000) - this->time_previous_state, 2)  + SPEED_MAX_ *(((double)millis()/1000) - this->time_previous_state) + this->bridge_pos ; 
            this->speed_cons = 0.80*(SPEED_MAX_ - ((this->acc)*((double)millis()/1000 - this->time_previous_state)));

            if (abs(this->pos - this->pos_target) <= this->epsilon){
                this->state = arret;

            };
            break;

        case arret:{
            this->pos_cons = this->pos_target;
            this->speed_cons=0;

            break;
        }
        };
    };

};

/*
void Command::set_state(){
    
    if (this->pos >= 0.90* this->pos_target){
        this->state = decceleration;}

    
    else if(this->speed_cons>=0.9*SPEED_MAX_){
        this->state = vitesse_cte;

    }
    else{

        this->state = acceleration;
    };
};
*/

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

double Command::get_pos_target(){
    return this->pos_target;
};