#define FOSC 16000000
#define BAUD 9600
#define MYUBRR 103//FOSC/(16 * BAUD) - 1

void UART_init(void);
void UART_transmit(unsigned char data);
unsigned char UART_receive();