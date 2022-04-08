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

double l_0 = 0;
int r_0 = 0;
int l_45 = 0;
int r_45 = 0;
int l_90 = 0;
int r_90 = 0;


double sharp_mm[39] = {150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95,  90,  85,  80,  75,  70,  65,  60,  55,  50,  45,  40,  35,  30, 25,  20,  18,  15,  14,  13,  12,  11,  10,  9,   8,   7,   6,   5};

double sharp_analog_l_90[39] = {87, 91, 93, 96, 99, 101, 103, 108, 112, 121, 125, 129, 141, 151, 159, 165, 176, 186, 196, 211, 227, 246, 261, 291, 337, 386, 454, 491, 571, 614, 651, 706, 761, 806, 891, 931, 956, 956, 926};

double sharp_analog_l_45[39] = {98, 99, 101, 103, 107, 110, 114, 116, 120, 129, 133, 137, 149, 159, 167, 173, 184, 194, 204, 219, 235, 254, 269, 299, 345, 394, 462, 499, 579, 622, 659, 714, 769, 814, 899, 939, 964, 964, 934};

double sharp_analog_l_0[39] = {98,  99,  101, 103, 107, 110, 115, 117, 121, 130, 134, 138, 150, 160, 168, 174, 185, 195, 205, 220, 236, 255, 270, 300, 346, 395, 463, 500, 580, 623, 660, 715, 770, 815, 900, 940, 965, 965, 935};

double sharp_analog_r_0[39] = {75, 80, 82, 86, 89, 91, 94, 96, 100, 109, 116, 121, 130, 135, 144, 149, 160, 164, 179, 189, 201, 215, 232, 255, 294, 340, 412, 446, 525, 567, 620, 660, 710, 758, 870, 925, 955, 953, 950};

double sharp_analog_r_45[39] = {98, 99, 101, 103, 107, 110, , 115, 137, 139, 148, 152, 156, 168, 178, 186, 192, 203, 213, 223, 238, 254, 273, 288, 318, 364, 413, 481, 518, 598, 641, 678, 733, 788, 833, 918, 958, 983, 983, 953};

double sharp_analog_r_90[39] = {75, 80, 82, 86, 89, 91, 94, 130, 134, 143, 150, 155, 164, 169, 178, 183, 194, 198, 213, 223, 235, 249, 266, 289, 328, 374, 446, 480, 559, 601, 654, 694, 744, 792, 904, 959, 989, 987, 984};




int sh_to_mm(int port, double analog_data)
{
  int i;
  double tga;


  switch (port)
  {
    case 1:
      i = 0;

      while (analog_data > sharp_analog_l_45[i])
        i++;
      tga = (sharp_analog_l_45[i + 1] - sharp_analog_l_45[i]) / (sharp_mm[i] - sharp_mm[i + 1]);
      l_45 = ((analog_data - sharp_analog_l_45[i]) / tga + sharp_mm[i]) * 10 ;
      if (l_45 > 1400)
        l_45 = 1400;
      if (l_45 < 110)
        l_45 = 110;
      break;

    case 0:
      i = 0;
      ANALOG_MUX(3);
      while (analog_data > sharp_analog_l_90[i])
        i++;
      tga = (sharp_analog_l_90[i + 1] - sharp_analog_l_90[i]) / (sharp_mm[i] - sharp_mm[i + 1]);
      l_0 = ((analog_data - sharp_analog_l_90[i]) / tga + sharp_mm[i]) * 10 ;
      if (l_90 > 1400)
        l_90 = 1400;
      if (l_90 < 110)
        l_90 = 110;
      break;

    case 2:
      i = 0;
      while (analog_data > sharp_analog_l_0[i])
        i++;
      tga = (sharp_analog_l_0[i + 1] - sharp_analog_l_0[i]) / (sharp_mm[i] - sharp_mm[i + 1]);
      l_0 = ((analog_data - sharp_analog_l_0[i]) / tga + sharp_mm[i]) * 10 ;
      if (l_0 > 1400)
        l_0 = 1400;
      if (l_0 < 110)
        l_0 = 110;
      break;

    case 3:

      i = 0;
      while (analog_data > sharp_analog_r_0[i])
        i++;
      tga = (sharp_analog_r_0[i + 1] - sharp_analog_r_0[i]) / (sharp_mm[i] - sharp_mm[i + 1]);
      r_0 = ((analog_data - sharp_analog_r_0[i]) / tga + sharp_mm[i]) * 10 ;
      if (r_0 > 1400)
        r_0 = 1400;
      if (r_0 < 110)
        r_0 = 110;
      break;

    case 4:
      i = 0;
      while (analog_data > sharp_analog_r_45[i])
        i++;
      tga = (sharp_analog_r_45[i + 1] - sharp_analog_r_45[i]) / (sharp_mm[i] - sharp_mm[i + 1]);
      r_45 = ((analog_data - sharp_analog_r_45[i]) / tga + sharp_mm[i]) * 10 ;
      if (r_45 > 1400)
        r_45 = 1400;
      if (r_45 < 110)
        r_45 = 110;
      break;


    case 5:
      i = 0;
      while (analog_data > sharp_analog_r_90[i])
        i++;
      tga = (sharp_analog_r_90[i + 1] - sharp_analog_r_90[i]) / (sharp_mm[i] - sharp_mm[i + 1]);
      r_90 = ((analog_data - sharp_analog_r_90[i]) / tga + sharp_mm[i]) * 10 ;
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

  sharp_read();
  Serial.print(l_90); Serial.print(" "); Serial.print(l_45); Serial.print(" "); Serial.print(l_0); Serial.print(" ");
  Serial.print(r_0); Serial.print(" "); Serial.print(r_45); Serial.print(" "); Serial.println(r_90);



}
