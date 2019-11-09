/*
#ifdef F_CPU
#undef F_CPU
#define F_CPU 4000000UL
#endif
*/
#define F_CPU 500000UL

//#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>  //For NOP()

#include "UART_driver.h"
#include "IO_driver.h"
#include "Timer.h"

//Used to define state
enum {COUNT, BLINK};

static volatile uint16_t counter_limit;
static volatile uint16_t counter = 0;
static volatile uint16_t blink_length = 2; //gives approx 0.5 seconds blink
static volatile uint8_t current_state = COUNT;
static volatile uint16_t moisture;
static uint16_t moisture_threshold = 100; //TODO test if this is a good value

int main(void) {
    cli();

    //initialize all IO
    IO_LED_init();
    IO_sensor_init();
    IO_switch_init();
    Timer_init();
    //IO_tick_init();
    _NOP();
   
    //read operation mode from switch, set counter limit from that
    uint8_t operationMode = IO_get_operation_mode();
    switch (operationMode)
    {
    case 1:
        //counter_limit = Timer_get_scaled_counter_limit(8);
        counter_limit = 4; //check every 2 seconds. For presentation
        break;
    
    case 0:
        counter_limit = 240; //check every 2 minutes. For use
        break;
    }

    //set sleep mode to power save, so it can be interrupted by timer2
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    

    //set power reduction to unused modules
    //TWI, Timer0, Timer1, SPI, USART0
    PRR |= (1<<PRTWI) | (1<<PRTIM0) | (1<<PRTIM1) | (1<<PRSPI) | (1<<PRUSART0);
    
    sei();
    while(1) {
        
        //TODO remove
        /*
        IO_toggle_LED();
        _delay_ms(1000);
        */
        
        sleep_mode();
    }

}

ISR(TIMER2_COMPA_vect) {
    counter++;
    //IO_toggle_LED();
    //IO_tick();
    switch (current_state)
    {
    case COUNT:
        
        if(counter == counter_limit) {
            //power up sensor
            IO_set_sensor_power(POWER_ON);
            //switch adc on
            IO_set_ADC_power(POWER_ON);
            //wait for adc and sensor to properly initialize
            _delay_ms(1000);
            //read moisture sensor
            moisture = IO_get_moisture();
            _NOP();
            //turn adc off
            IO_set_ADC_power(POWER_OFF);
            //power down sensor
            IO_set_sensor_power(POWER_OFF);
            //if dry -> blink
            if(moisture < moisture_threshold) {
                current_state = BLINK;
                IO_set_LED_power(POWER_ON);
            }
            counter = 0;
        }
        break;
    
    case BLINK:
        if(counter == blink_length) {
            IO_set_LED_power(POWER_OFF);
            current_state = COUNT;
            counter = 0;
        }
        break;
    }
    
}

