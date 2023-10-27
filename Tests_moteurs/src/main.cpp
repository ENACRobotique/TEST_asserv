#include <Arduino.h>

#include "high_gain_filter.h"
#include <STM32FreeRTOS.h>
#include <math.h>
#include "PID.h"
#include "command.h"

#define MOTOR_1_PWM PB4
#define MOTOR_1_DIR PB1
#define MOTOR_2_PWM PB5
#define MOTOR_2_DIR PB2
#define MOTOR_3_PWM PB10
#define MOTOR_3_DIR PB15

#define ENCODER_1_A PC14
#define ENCODER_1_B PC13
#define ENCODER_2_A PC3
#define ENCODER_2_B PC2
#define ENCODER_3_A PC9 
#define ENCODER_3_B PC8

#define DISPLAY_CLK PB11
#define DISPLAY_DATA PB12
#define COLOR PC6
#define POS_BUTTON PC5
#define TIRETTE PC15

#define INCREMENT_TO_MM 0.2605932


constexpr float POS_ALPHA0 = 2.4f;
constexpr float POS_ALPHA1 = 2.08f;
constexpr float POS_ALPHA2 = 0.64f;
constexpr float POS_EPS = 0.04f; // 0.02

float alphas[] = {POS_ALPHA0, POS_ALPHA1, POS_ALPHA2};

int32_t counter = 0;
int32_t prev_counter = 0;
uint32_t last_time = 0;
uint32_t last_time_filter = 0;

int32_t position = 0;
high_gain_filter pos_filter;


//int32_t error =0;
//int32_t pos_cons=0;
//int32_t speed_cons=0;
//int32_t pos_target;

float dt = 25;


PID pid;
Command commande ;


void posFilterTask(void *arg) {
  while(true) {
    TickType_t t = xTaskGetTickCount();
    taskENTER_CRITICAL();
    high_gain_filter_process(&pos_filter, counter * INCREMENT_TO_MM/1000);
    taskEXIT_CRITICAL();
    vTaskDelayUntil(&t, pdMS_TO_TICKS(1));
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

  high_gain_filter_init(&pos_filter, alphas, POS_EPS, 1000 );

  xTaskCreate(posFilterTask, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  
  vTaskStartScheduler();
  commande.set_pos_target(3.0);
  commande.set_acc();
  
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

  
  
  if(millis() - last_time > dt && commande.get_pos()<=3.0) {

    commande.set_pos(pos_filter.hatx[0]);
    commande.set_speed(pos_filter.hatx[1]);

    commande.set_cons();

    commande.set_error();
    
    
    commande.set_state();

    pid.set_derivative(commande.get_speed_cons(),commande.get_speed());
    pid.set_integral(commande.get_error(),dt/1000);
    
    commande.set_cmd(pid.update_cmd(commande.get_error())) ;

    /*
    Serial.print( pid.update_cmd(commande.get_error()));
    Serial.println(" ");
    */
    
    setCommand(commande.get_cmd());
    
    last_time = millis();
    float speed_est = (counter-prev_counter)*INCREMENT_TO_MM/1000/(dt/1000);

    Serial.print((int)commande.get_state());
    Serial.print(" ");
    Serial.print(commande.get_pos());
    Serial.print(" ");
    Serial.print(commande.get_pos_cons());
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(commande.get_speed());
    Serial.print(" ");
    Serial.println(commande.get_speed_cons());

    prev_counter = counter;
  }
Serial.print((int)commande.get_state());
    Serial.print(" ");
    Serial.print(commande.get_pos());
    Serial.print(" ");
    Serial.print(commande.get_pos_cons());
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(commande.get_speed());
    Serial.print(" ");
    Serial.println(commande.get_speed_cons());
}

