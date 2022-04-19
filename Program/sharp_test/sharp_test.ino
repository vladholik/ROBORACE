#include <VL53L0X.h>
#include <Wire.h>
#include <Servo.h>
#include <Math.h>
#include <SPI.h>



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

double l_0 = 0;
double r_0 = 0;
double l_45 = 0;
double r_45 = 0;
double l_90 = 0;
double r_90 = 0;


int sh_to_mm(int port, double analog_data)
{
  int i;
  double tga;


  switch (port)
  {
    case 1:
      l_45 = 75834 * pow(analog_data, -1.341)* 10;
      if (l_45 > 1400)
        l_45 = 1400;
      if (l_45 < 110)
        l_45 = 110;
      break;

    case 0:
    l_90 = 75834 * pow(analog_data, -1.341)* 10;
      if (l_90 > 1400)
        l_90 = 1400;
      if (l_90 < 110)
        l_90 = 110;
      break;

    case 2:
     l_0 = 75834 * pow(analog_data, -1.341)* 10;
      if (l_0 > 1400)
        l_0 = 1400;
      if (l_0 < 110)
        l_0 = 110;
      break;

    case 3:

      r_0 = 75834 * pow(analog_data, -1.341)* 10;
      if (r_0 > 1400)
        r_0 = 1400;
      if (r_0 < 110)
        r_0 = 110;
      break;

    case 4:
     r_45 = 75834 * pow(analog_data, -1.341)* 10;
      if (r_45 > 1400)
        r_45 = 1400;
      if (r_45 < 110)
        r_45 = 110;
      break;


    case 5:
      r_90 = 75834 * pow(analog_data, -1.341)* 10;
      if (r_90 > 1400)
        r_90 = 1400;
      if (r_90 < 110)
        r_90 = 110;
      break;


  }
}

void sharp_read()
{
  ANALOG_MUX(3);
  sh_to_mm(0, analogRead(ANALOG_READ_PORT));

  ANALOG_MUX(2);
  sh_to_mm(1, analogRead(ANALOG_READ_PORT));

  ANALOG_MUX(1);
  sh_to_mm(2, analogRead(ANALOG_READ_PORT));

  ANALOG_MUX(0);
  sh_to_mm(3, analogRead(ANALOG_READ_PORT));

  ANALOG_MUX(6);
  sh_to_mm(4, analogRead(ANALOG_READ_PORT));

  ANALOG_MUX(7);
  sh_to_mm(5, analogRead(ANALOG_READ_PORT));
}

void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70); // TCA9548A адрес 0x70
  Wire.write(1 << bus); // отправляем байт на выбранную шину
  Wire.endTransmission();
}


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

  sharp_read();
  Serial.print(l_90); Serial.print(" "); Serial.print(l_45); Serial.print(" "); Serial.print(l_0); Serial.print(" ");
  Serial.print(r_0); Serial.print(" "); Serial.print(r_45); Serial.print(" "); Serial.println(r_90);



}
