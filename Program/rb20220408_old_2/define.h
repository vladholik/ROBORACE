#include <Arduino.h>

#define OLED_ADDR   0x3C
#define OLED_RESET  -1        // no reset pin
#define SYSTEM_INTERRUPT_CLEAR       0x0B
#define RESULT_INTERRUPT_STATUS      0x13
#define RESULT_RANGE_STATUS          0x14

#define NEUTRAL 90
#define REVERSE 63
#define S0 96

#define ENCODER_PORT PIN_PC0
#define ANALOG_READ_PORT PIN_PC1
#define INTEGRAL_DATA_3 PIN_PD7
#define INTEGRAL_DATA_2 PIN_PB0
#define INTEGRAL_DATA_1 PIN_PB1


long c = 0;
int f = 0;
double spos = 0;
int encoder = 0;
double sp = 0;
double t_sp = 24;
double f_sp = 28;
double f_f_sp = 36;
