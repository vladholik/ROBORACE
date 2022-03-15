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
#define S0 100

#define ENCODER_PORT PIN_PC0

VL53L0X tof_0, tof_1, tof_2, tof_3, tof_4, tof_5;
Servo servo, motor;  // создадим объект сервопривода и мотора
Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);

bool setup_6_tofs(int timeout) {
  
  TCA9548A(6);
  Serial.println("Подготовка tof-модулей ...");
  if (tof_0.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 0!");
   return 0;}
   tof_0.setTimeout(timeout);
   tof_0.setSignalRateLimit(0.1);
   tof_0.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
   tof_0.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
   tof_0.startContinuous();
   
  TCA9548A(1);
  if (tof_1.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 1!");
   return 0;}
   tof_1.setTimeout(timeout);
   tof_1.startContinuous();
   
   TCA9548A(2);
  if (tof_2.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 2!");
    return 0;}
   tof_2.setTimeout(timeout);
   tof_2.startContinuous();
   
   TCA9548A(3);
   if (tof_3.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 3!");
    return 0;}
   tof_3.setTimeout(timeout);
   tof_3.setSignalRateLimit(0.1);
   tof_3.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
   tof_3.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
   tof_3.startContinuous();
   
   TCA9548A(4);
  if (tof_4.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 4!");
    return 0;}
   tof_4.setTimeout(timeout);
   tof_4.startContinuous();
   

   TCA9548A(7);
  if (tof_5.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 5!");
    return 0;}
   tof_5.setTimeout(timeout);
   tof_5.startContinuous();
      
   Serial.println("success");
   return 0;
  }
  
void TCA9548A(uint8_t bus)
{
 Wire.beginTransmission(0x70); // TCA9548A адрес 0x70
 Wire.write(1 << bus); // отправляем байт на выбранную шину
 Wire.endTransmission();
}


int spos = 0;
int l_0 = 0;
int r_0 = 0;
int l_45 = 0;
int r_45 = 0;
int l_90 = 0;
int r_90 = 0;
int tofs[6];
long c = 0;
int f = 0;
int err_45 = 0;
int err_90 = 0;
int encoder = 0;




void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  TCA9548A(5);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
//  display.clearDisplay();
//  //
//  display.setCursor(25, 10);
//  display.print("SUCCESS");
//  display.display();
  
  servo.attach(PIN_PD5);  
  motor.attach(PIN_PB2); 
  pinMode(ENCODER_PORT, INPUT);
  //motor.write(99);
  
  
  if (setup_6_tofs(1000) != 0) {
    while (1) {}
  }
}


double rpm = 0;
int state = 1;
unsigned long int tt;
int magnet = 0;

int velocity_count()
{
  if (analogRead(ENCODER_PORT) > 520)
    magnet = 1;
  else 
    magnet = 0;

  if(millis()-tt >750)
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



void tof_read(){  
  
  TCA9548A(6);
  //Serial.println("6");
  if (0 != (tof_0.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        r_0 = tof_0.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_0.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
        //Serial.println(r_0);
        if (r_0 > 2200)
          r_0 = 2200;
    }

  TCA9548A(1);
  if (0 != (tof_1.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        l_45 = tof_1.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_1.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
        if (l_45 > 1200)
          l_45 = 1200;
    }

  TCA9548A(2);
  if (0 != (tof_2.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        r_45 = tof_2.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_2.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
        if (r_45 > 1200)
          r_45 = 1200;
    }
    
  TCA9548A(3);
  if (0 != (tof_3.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        l_0 = tof_3.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_3.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
        if (l_0 > 2200)
          l_0 = 2200;
    }

  TCA9548A(4);
  if (0 != (tof_4.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        l_90 = tof_4.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_4.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01); 
        if(l_90 > 1200)
          l_90 = 1200; 
    }

  TCA9548A(7);
  if (0 != (tof_5.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        r_90 = tof_5.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_5.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01); 
        if(r_90 > 1200)
          r_90 = 1200; 
    }

  err_45 = r_45-l_45;
  err_90 = r_90-l_90;
 
}

void serial_print()
{
  Serial.print(" ");
  Serial.print(l_90);
  Serial.print(" ");
  Serial.print(l_45);
  Serial.print(" ");
  Serial.print(l_0);
  Serial.print(" ");
  Serial.print(r_0);
  Serial.print(" ");
  Serial.print(r_45);
  Serial.print(" ");
  Serial.println(r_90);
}

void display_print()
{
  TCA9548A(5);  
  //display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();        
  display.setCursor(0, 23);
  display.print(l_90);
  display.setCursor(103, 23);
  display.print(r_90);
  display.setCursor(10, 10);
  display.print(l_45);
  display.setCursor(100, 10);
  display.print(r_45);
  display.setCursor(45, 0);
  display.print(l_0);
  display.setCursor(70, 0);
  display.print(r_0);  

  
  display.display();
}




//------------------------------------------------------------------------------------------------------------------------
void loop() { 
  
  servo.write(100);
  delay(5000);

  
  while(1){
    motor.write(100);

     
      tof_read();
      velocity_count();
      
      Serial.print(analogRead(ENCODER_PORT));
      serial_print();
      display_print();
      
  
    if (l_0 < 120 && l_0 > 10 || r_0 < 120 && r_0 > 10){
      servo.write(200-spos);
      motor.write(NEUTRAL);
      delay(50);      
      motor.write(REVERSE);
      delay(150);
      motor.write(NEUTRAL);
      delay(50);
      motor.write(REVERSE);
      delay(500);
      motor.write(NEUTRAL);
    }
    else{



      if (r_0 > 1200 || l_0 > 1200){
        //forward
        motor.write(100);
        servo.write(S0);
        spos = S0;
      } else{
        //turn
        motor.write(100);
        
        if (r_90 > 1000){
          //right
          servo.write(S0-30);
          motor.write(REVERSE);
          delay(60);
          motor.write(100);
          spos = S0-30;}
          
        else{ if (l_90 > 1000){
                //left
                servo.write(S0+30);
                motor.write(REVERSE);
                delay(60);
                motor.write(100);
                spos = S0+30;
                
        }     else if (r_45 > 1000){
                    // light right
                    servo.write(S0-15); 
                    motor.write(REVERSE);
                    delay(20);
                    motor.write(100);
                    //motor.write(NEUTRAL-10);
                    spos = S0-15;
            } else if (l_45 > 1000){
                    //light left
                      servo.write(S0+15);
                      delay(20);
                      motor.write(100);
                      //motor.write(NEUTRAL-10);
                      spos = S0+15;
            } else{
              motor.write(100);
            }
                
            
          }
        
        }
    }
    

    

    
      }
    
}
