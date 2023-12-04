#include <Arduino.h>

#include "high_gain_filter.h"
#include <STM32FreeRTOS.h>
#include <math.h>

#include "command.h"
#include "config.h"




int32_t counter = 0;
int32_t prev_counter = 0;
uint32_t last_time = 0;
uint32_t last_time_filter = 0;

int32_t position = 0;



//int32_t error =0;
//int32_t pos_cons=0;
//int32_t speed_cons=0;
//int32_t pos_target;

float dt = 25;



Command commande(3.6) ;


void posFilterTask(void *arg) {
  while(true) {
    TickType_t t = xTaskGetTickCount();
    taskENTER_CRITICAL();
    high_gain_filter_process(&pos_filter, counter * INCREMENT_TO_MM/1000);
    taskEXIT_CRITICAL();
    vTaskDelayUntil(&t, pdMS_TO_TICKS(2));
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_1_PWM, OUTPUT);
  pinMode(MOTOR_1_DIR, OUTPUT);
  pinMode(ENCODER_1_A, INPUT_PULLUP);
  pinMode(ENCODER_1_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_1_A), [=]() {
      if (digitalRead(ENCODER_1_B) == HIGH) {
          counter++;
      } else {
          counter--;
      }
  }, RISING);

  

  xTaskCreate(posFilterTask, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  
  vTaskStartScheduler();
  
}


void setCommand(int cmd) {
  if(cmd < -255) {
    cmd = -255;
  }
  if(cmd > 255) {
    cmd = 255;
  }

  if(cmd >= 0) {
    digitalWrite(MOTOR_1_DIR, HIGH);
  } else {
    digitalWrite(MOTOR_1_DIR, LOW);
  }
  analogWrite(MOTOR_1_PWM, abs(cmd));
}

void loop() {

  
  
  if(millis() - last_time > dt){ //&& commande.get_pos()<=3.0) {

    commande.set_pos((double) pos_filter.hatx[0]);
    commande.set_speed((double) pos_filter.hatx[1]);

    commande.set_cons();

    commande.set_error();
    

    pid.set_derivative(commande.get_speed_cons(),commande.get_speed());
    pid.set_integral(commande.get_error(),dt/1000);
    
    commande.set_cmd(pid.update_cmd(commande.get_error())) ;

    /*
    Serial.print( pid.update_cmd(commande.get_error()));
    Serial.println(" ");
    */
    
    setCommand(commande.get_cmd());
    
    last_time = millis();
    //float speed_est = (counter-prev_counter)*INCREMENT_TO_MM/1000/(dt/1000);

    Serial.print((int)commande.get_state());//1
    Serial.print(" ");
    Serial.print(commande.get_pos());//2
    Serial.print(" ");
    Serial.print(commande.get_pos_cons());//3
    Serial.print(" ");
    Serial.print(commande.get_speed());//4
    Serial.print(" ");
    Serial.print(commande.get_error());//5
    Serial.print(" ");
    Serial.print(commande.get_speed_cons());//-commande.get_speed());//6
    Serial.print(" ");
    Serial.print(commande.get_pos_target());//7
    Serial.print(" ");
   // Serial.print(commande.get_acc());//8
    Serial.print(" ");
    //Serial.println(millis()/1000);//9
    Serial.println(" ");
    prev_counter = counter;
  }
  /*
    Serial.print((int)commande.get_state());
    Serial.print(" ");
    Serial.print(commande.get_pos());
    Serial.print(" ");
    Serial.print(commande.get_pos_cons());
    Serial.print(" ");
    Serial.println(commande.get_speed());
    //Serial.print(" ");
    //Serial.println(commande.get_speed_cons());
    */
}

