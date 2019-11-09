#include <avr/io.h>
#define POWER_ON 1
#define POWER_OFF 0

void IO_sensor_init();
void IO_set_sensor_power(uint8_t power);
void IO_set_ADC_power(uint8_t power);
uint16_t IO_get_moisture();

void IO_LED_init();
void IO_set_LED_power(uint8_t power);
void IO_toggle_LED();

void IO_switch_init();
uint8_t IO_get_operation_mode();

void IO_tick_init();
void IO_tick();
