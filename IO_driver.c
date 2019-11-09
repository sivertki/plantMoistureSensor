#include "IO_driver.h"


void IO_sensor_init(){
    //set data direction of power pin of sensor to output
    DDRC |= (1<<PC0);
    //set voltage reference to AVCC
    ADMUX |= (1<<REFS0);
    //select input ADC1 to connect to converter
    ADMUX |= (1<<MUX0);
}

void IO_LED_init() {
    //Define port as output
    DDRC |= (1<<PC2); 
}

void IO_switch_init(){
    //define port as input
    DDRD &= ~(1<<PD2);
}

void IO_set_sensor_power(uint8_t power) {
    if(power == POWER_ON) {
        PORTC |= (1<<PC0);
    }
    else if(power == POWER_OFF) {
        PORTC &= ~(1<<PC0);
    }
}

uint16_t IO_get_moisture() {
    //set bit to start single conversion
    ADCSRA |= (1<<ADSC);
    //wait for result
    while((ADCSRA & (1<<ADSC)));
    //read result
    uint16_t ADC_value = ADC;
    //return result
    return ADC_value;
}

void IO_set_ADC_power(uint8_t power) {
    switch (power)
    {
    case POWER_OFF:
        //disable ADC
        ADCSRA &= ~(1<<ADEN);
        //power reduction ADC
        PRR |= (1<<PRADC);
        break;
    
    case POWER_ON:
        //remove power reduction 
        PRR &= ~(1<<PRADC);
        //enable ADC
        ADCSRA |= (1<<ADEN);
        break;
        
    }
}

/*
Returns 1 if input to PD2 is high, 0 if low.
Used to determine mode of operation.
*/
uint8_t IO_get_operation_mode(){
    uint8_t input = PIND;
    if(input & (1<<PIN2)){
        return 1;
    }
    else {
        return 0;
    }
}

void IO_set_LED_power(uint8_t power){
    if(power == POWER_ON) {
        PORTC |= (1<<PC2);
    }
    else if(power == POWER_OFF) {
        PORTC &= ~(1<<PC2);
    }
}

void IO_toggle_LED() {
    PORTC ^= (1<<PC2);
}

void IO_tick_init() {
    //PD3
    //Define port as output
    DDRD |= (1<<PD3); 
}

void IO_tick() {
    PORTD ^= (1<<PD3);
}