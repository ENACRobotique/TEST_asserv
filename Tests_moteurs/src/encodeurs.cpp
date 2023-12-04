
#include "encodeurs.h"
#include <cstdint>
#include "config.h"
#include "stm32_tim.h"

constexpr float POS_ALPHA0 = 2.4f;
constexpr float POS_ALPHA1 = 2.08f;
constexpr float POS_ALPHA2 = 0.64f;
constexpr float POS_EPS = 0.04f; // 0.02

std::array<double, 3> alphas = {POS_ALPHA0, POS_ALPHA1, POS_ALPHA2};


Encoder::Encoder(stm32_tim_t* tim, double inc_to_mm):
tim(tim), offset(0),
lower_half(true), inc_to_mm(inc_to_mm)
{
  pos_filter.init(alphas, POS_EPS, 40);
}

void Encoder::init(bool inverted) {
    pinMode(ENCODER_1_A, INPUT_PULLUP);
    pinMode(ENCODER_1_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENCODER_1_A), [=]() {
      if (digitalRead(ENCODER_1_B) == HIGH) {
          counter++;
      } else {
          counter--;
      }
  }, RISING);
}

int32_t Encoder::get_value() {
  uint32_t counter_val = tim->CNT;

  if(tim->SR & TIM_SR_UIF) {  //an overflow/underflow happend 
    if(counter_val <= 0x7FFF && !lower_half) {
      // the counter is in the lower half, and was previously in the upper half: overflow
      offset += 1<<16;
    } else if(counter_val > 0x7FFF && lower_half) {
      // the counter is in the upper half, and was previously in the lower half: underflow
      offset -= 1<<16;
    }
    // if it's not one of the abover cases, an overflow/underflow happened,
    // but the counter did the opposite underflow/overflow,
    // so nothing needs to be done.

    tim->SR = 0;   // reset interrupt flag
  }
  
  if(counter_val <= 0x7FFF) {
    lower_half = true;
  }
  else {
    lower_half = false;
  }

  return static_cast<int32_t>(counter_val) + offset;
}

void Encoder::update_filter()
{
  pos_filter.process(get_value() / inc_to_mm);
}