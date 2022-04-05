#include <VL53L0X.h>
#include <Wire.h>
#include <Servo.h>
#include <Math.h>
#include <SPI.h>
//#include <avr_stl.h>


#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
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

int l_0 = 0;
int r_0 = 0;
int l_45 = 0;
int r_45 = 0;
int l_90 = 0;
int r_90 = 0;


int sharp_mm[39] = {150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95,  90,  85,  80,  75,  70,  65,  60,  55,  50,  45,  40,  35,  30, 25,  20,  18,  15,  14,  13,  12,  11,  10,  9,   8,   7,   6,   5};

int sharp_analog_l_0[39] = {98,  99,  101, 103, 107, 110, 115, 117, 121, 130, 134, 138, 150, 160, 168, 174, 185, 195, 205, 220, 236, 255, 270, 300, 346, 395, 463, 500, 580, 623, 660, 715, 770, 815, 900, 940, 965, 965, 935};

int sharp_analog_r_0[39] = {75, 80, 82, 86, 89, 91, 94, 96, 100, 109, 116, 121, 130, 135, 144, 149, 160, 164, 179, 189, 201, 215, 232, 255, 294, 340, 412, 446, 525, 567, 620, 660, 710, 758, 870, 925, 955, 953, 950};




void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70); // TCA9548A адрес 0x70
  Wire.write(1 << bus); // отправляем байт на выбранную шину
  Wire.endTransmission();
}


//int sh_to_mm (int port, int analog



void ANALOG_MUX(uint8_t port)
{
  switch (port)
  {
    case 0:
    digitalWrite(INTEGRAL_DATA_1, 0);
    digitalWrite(INTEGRAL_DATA_2, 0);
    digitalWrite(INTEGRAL_DATA_3, 0);
    break;

    case 1:
    digitalWrite(INTEGRAL_DATA_1, 0);
    digitalWrite(INTEGRAL_DATA_2, 0);
    digitalWrite(INTEGRAL_DATA_3, 1);
    break;

    case 2:
    digitalWrite(INTEGRAL_DATA_1, 0);
    digitalWrite(INTEGRAL_DATA_2, 1);
    digitalWrite(INTEGRAL_DATA_3, 0);
    break;

    case 3:
    digitalWrite(INTEGRAL_DATA_1, 0);
    digitalWrite(INTEGRAL_DATA_2, 1);
    digitalWrite(INTEGRAL_DATA_3, 1);
    break;

    case 4:
    digitalWrite(INTEGRAL_DATA_1, 1);
    digitalWrite(INTEGRAL_DATA_2, 0);
    digitalWrite(INTEGRAL_DATA_3, 0);
    break;

    case 5:
    digitalWrite(INTEGRAL_DATA_1, 1);
    digitalWrite(INTEGRAL_DATA_2, 0);
    digitalWrite(INTEGRAL_DATA_3, 1);
    break;

    case 6:
    digitalWrite(INTEGRAL_DATA_1, 1);
    digitalWrite(INTEGRAL_DATA_2, 1);
    digitalWrite(INTEGRAL_DATA_3, 0);
    break;

    case 7:
    digitalWrite(INTEGRAL_DATA_1, 1);
    digitalWrite(INTEGRAL_DATA_2, 1);
    digitalWrite(INTEGRAL_DATA_3, 1);
    break;
  }
  
}


void setup() {
  pinMode(ANALOG_READ_PORT, INPUT);
  pinMode(INTEGRAL_DATA_1, OUTPUT);
  pinMode(INTEGRAL_DATA_2, OUTPUT);
  pinMode(INTEGRAL_DATA_3, OUTPUT);
  Serial.begin(9600);

}

void loop() {

    ANALOG_MUX(3);
    Serial.print(" l90 : ");
    Serial.print(analogRead(ANALOG_READ_PORT));
    
    ANALOG_MUX(2);
    Serial.print(" l45 : ");
    Serial.print(analogRead(ANALOG_READ_PORT));

    ANALOG_MUX(1);
    Serial.print(" l0 : ");
    Serial.print(analogRead(ANALOG_READ_PORT));
  
    ANALOG_MUX(0);
    Serial.print(" r0 : ");
    Serial.print(analogRead(ANALOG_READ_PORT));

    ANALOG_MUX(6);
    Serial.print(" r45 : ");
    Serial.print(analogRead(ANALOG_READ_PORT));

    ANALOG_MUX(7);
    Serial.print(" r90 : ");
    Serial.print(analogRead(ANALOG_READ_PORT));
    
    Serial.println("   ");

    

}
