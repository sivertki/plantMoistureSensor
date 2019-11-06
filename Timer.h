#include <avr/interrupt.h>

void Timer_init();
uint16_t Timer_get_scaled_counter_limit(uint8_t seconds);