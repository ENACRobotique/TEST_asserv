#pragma once

#include "high_gain_filter.h"

class Encoder
{

private:
    stm32_tim_t* tim;
    int32_t offset;
    
    // is the counter in the lower or upper half? Will be used to detect overflow/underflow
    bool lower_half;

    high_gain_filter pos_filter;
    double inc_to_mm;

    
public:
    Encoder(stm32_tim_t* tim, double inc_to_mm);
    void init(bool inverted);

    // return offset corrected encoder value
    int32_t get_value();
    void update_filter();
    //void reset();

    // return filtered position
    double get_pos() { return pos_filter.get_pos(); }

    //return filtered speed
    double get_speed()  { return pos_filter.get_speed(); }


};