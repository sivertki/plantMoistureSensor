#include "Timer.h"

void Timer_init() {
    //set mode to CTC
    TCCR2A |= (1<<WGM21);
    //set prescaler to 1024
    TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);

    //250 in binary. With a 4MHz oscillator, each output compare match
    //with 1024 prescaler is equavalent to 64ms. Enables the ability to count
    //seconds divisible by 8.
    OCR2A = 0b11111010;
    
    //enable interrupt on ouput compare match A
    TIMSK2 |= (1<<OCIE2A);
}

//takes in seconds, returns how many interrups need to be counted.
//Maximum 524 seconds
uint16_t Timer_get_scaled_counter_limit(uint8_t seconds) {
    uint16_t max_count = (seconds * 125) / 8;
    return max_count;
}