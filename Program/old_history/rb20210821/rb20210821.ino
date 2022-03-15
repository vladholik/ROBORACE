#include <VL53L0X.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


#define SYSTEM_INTERRUPT_CLEAR       0x0B
#define RESULT_INTERRUPT_STATUS      0x13
#define RESULT_RANGE_STATUS          0x14


// OLED display I2C address
#define OLED_ADDR   0x3C
#define OLED_RESET  -1        // no reset pin
#define TOF_1_ADDR  0x29
#define TOF_2_ADDR  0x2B
#define TOF_3_ADDR  0x2C
#define TOF_4_ADDR  0x2D
#define TOF_5_ADDR  0x2E
#define TOF_6_ADDR  0x2F



#define NEUTRAL 90
#define REVERSE 63

Servo servo, motor;  // создадим объект сервопривода и мотора
VL53L0X tof_0, tof_1, tof_2, tof_3, tof_4, tof_5;

Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);

bool setup_6_tofs(int timeout);

void TCA9548A(uint8_t bus)
{
 Wire.beginTransmission(0x70); // TCA9548A адрес 0x70
 Wire.write(1 << bus); // отправляем байт на выбранную шину
 Wire.endTransmission();
}

void dislplay_print_6_tofs(int l_0, int r_0, int l_45, int r_45, int l_90, int r_90) {
  TCA9548A(7);        
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
  //delay(500);
  display.clearDisplay();
}


void setup() {
  servo.attach(PD5);  // сервопривод на выводе 9
  motor.attach(10); // мотор на выводе 10
  motor.write(99);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  //display.setRotation(2);
  //display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  //  display.drawPixel(10, 10, SSD1306_WHITE);
  //  display.display();
  //  display.clearDisplay();
  //display.setCursor(0, 0);


  while (!Serial) {
    ; // ожидаем подключения монитора порта
  }
  Serial.println("OK");
  if (setup_6_tofs(1000) != 0) {
    while (1) {}
  }

  Serial.println("Arduino setup has been successfully finished!");
}

int i = 0;
double kp = 0;
int max_distance = 1000;
int l_0 = 0;
int r_0 = 0;
int l_45 = 0;
int r_45 = 0;
int l_90 = 0;
int r_90 = 0;
int err = 0;
int sr = 0;

void loop() {
  TCA9548A(0);
  if (0 != (tof_0.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        r_0 = tof_0.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_0.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
    }

  TCA9548A(1);
  if (0 != (tof_1.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        l_45 = tof_1.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_1.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
    }

  TCA9548A(2);
  if (0 != (tof_2.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        r_45 = tof_2.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_2.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
    }
  TCA9548A(3);
  if (0 != (tof_3.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        l_0 = tof_3.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_3.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
    }

  TCA9548A(4);
  if (0 != (tof_4.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        l_90 = tof_4.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_4.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);  
    }

  TCA9548A(5);
  if (0 != (tof_5.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        r_90 = tof_5.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_5.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);  
    }


  dislplay_print_6_tofs(l_0, r_0, l_45, r_45, l_90, r_90);

  kp = 0.05;
  motor.write(103);

  if (r_90 > max_distance) {
    motor.write(103);
    servo.write(147);
    
  }
  
  if (l_90 > max_distance) {
    motor.write(103);
    servo.write(33);
  }


  if ((l_90 < max_distance) and (r_90 < max_distance)) {
    motor.write(99);
    servo.write(100 + err * kp);
    Serial.println(err * kp);
  }
  if ((sr < 200) or (r_45 < 100) or (l_45< 100) or (l_0 < 100) or (r_0 < 100)){
    if (r_90 < l_90){
      servo.write(130);}
    else {
      servo.write(40);
    }
    delay(1000);
    motor.write(NEUTRAL);
    delay(200);
    motor.write(REVERSE);
    delay(200);
    motor.write(NEUTRAL);
    delay(200);
    motor.write(REVERSE);
    delay(500);
    motor.write(NEUTRAL);
    delay(200);
  }



  //servo.write(35); //полный право
  // servo.write(145); //полный лево




}
bool setup_6_tofs(int timeout) {
  
  TCA9548A(0);
  Serial.println("Подготовка tof-модулей ...");
  if (tof_0.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 0!");
   return 1;}
   tof_0.setTimeout(timeout);
   tof_0.startContinuous();
   
  TCA9548A(1);
  if (tof_1.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 1!");
   return 1;}
   tof_1.setTimeout(timeout);
   tof_1.startContinuous();
   
   TCA9548A(2);
  if (tof_2.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 2!");
    return 1;}
   tof_2.setTimeout(timeout);
   tof_2.startContinuous();
   
   TCA9548A(3);
   if (tof_3.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 3!");
    return 1;}
   tof_3.setTimeout(timeout);
   tof_3.startContinuous();
   
   TCA9548A(4);
  if (tof_4.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 4!");
    return 1;}
   tof_4.setTimeout(timeout);
   tof_4.startContinuous();

   TCA9548A(5);
  if (tof_5.init() == 0){
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 5!");
    return 1;}
   tof_5.setTimeout(timeout);
   tof_5.startContinuous();
   
   Serial.println("success");
   return 0;
  }
