#include <VL53L0X.h>  
#include <Wire.h>  
#include <Servo.h>
#include <Math.h>
#include <time.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_ADDR   0x3C
#define OLED_RESET  -1        // no reset pin
#define SYSTEM_INTERRUPT_CLEAR       0x0B
#define RESULT_INTERRUPT_STATUS      0x13
#define RESULT_RANGE_STATUS          0x14

#define NEUTRAL 90
#define REVERSE 63
#define S0 100

#define ENCODER_PORT PIN_PC0

Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);


void setup() {
  TCA9548A(0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  Serial.begin(9600);
  Wire.begin();
  pinMode(ENCODER_PORT, INPUT);

}

void TCA9548A(uint8_t bus)
{
 Wire.beginTransmission(0x70); // TCA9548A адрес 0x70
 Wire.write(1 << bus); // отправляем байт на выбранную шину
 Wire.endTransmission();
}


double rpm = 0;
int state = 1;
unsigned long int tt;
int magnet = 0;

int velocity_count()
{
  if (analogRead(ENCODER_PORT) > 500)
    magnet = 1;
  else 
    magnet = 0;

  if(millis()-tt > 2000)
    rpm = 0;
    
  switch (state){
  
  case 1:
    if (magnet == 1){
      tt = millis();
      state = 2;}
  break;

  case 2:
      if (magnet == 0)
        state = 3;
      
  break;

  case 3:
    if (magnet == 1){
      rpm = 1000/4/double(millis()-tt);
      state = 1;
    }
  break;
  }
}

void display_print()
{
  TCA9548A(0);  
       
  //display.clearDisplay(); 
  display.setCursor(0, 23);
  display.print(1);
  display.display();
}

void loop() {
  velocity_count();
  Serial.println(rpm);
  display_print();  

}
