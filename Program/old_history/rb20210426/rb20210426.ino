#include <VL53L0X.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


#define SYSTEM_INTERRUPT_CLEAR       0x0B
#define RESULT_INTERRUPT_STATUS      0x13
#define RESULT_RANGE_STATUS          0x14

#define XSHUT_TOF_1  2
#define XSHUT_TOF_2  3
#define XSHUT_TOF_3  4
#define XSHUT_TOF_4  5

// OLED display I2C address
#define OLED_ADDR   0x3C
#define OLED_RESET  -1        // no reset pin
#define TOF_1_ADDR  0x29
#define TOF_2_ADDR  0x2B
#define TOF_3_ADDR  0x2C
#define TOF_4_ADDR  0x2D


#define NEUTRAL 90
#define REVERSE 63

Servo servo, motor;  // создадим объект сервопривода и мотора
VL53L0X tof_1;
VL53L0X tof_2;
VL53L0X tof_3;
VL53L0X tof_4;
Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);

bool setup_4_tofs(int timeout);

void dislplay_print_4_tofs(int tof1_distance, int tof2_distance, int tof3_distance, int tof4_distance) {
  display.setCursor(0, 0);
  display.print(tof4_distance);
  display.setCursor(25, 0);
  display.print(tof3_distance);
  display.setCursor(50, 0);
  display.print(tof2_distance);
  display.setCursor(75, 0);
  display.print(tof1_distance);
  display.display();
  //delay(100);
  display.clearDisplay();
}


void setup() {
  servo.attach(9);  // сервопривод на выводе 9
  motor.attach(10); // мотор на выводе 10
  motor.write(99);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  delay(2000);
  display.clearDisplay();
  //display.setRotation(2);
  //display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  //  display.drawPixel(10, 10, SSD1306_WHITE);
  //  display.display();
  delay(2000);
  //  display.clearDisplay();
  //display.setCursor(0, 0);


  while (!Serial) {
    ; // ожидаем подключения монитора порта
  }
  Serial.println("OK");
  if (setup_4_tofs(1000) != 0) {
    while (1) {}
  }

  Serial.println("Arduino setup has been successfully finished!");
}

int i = 0;
double kp = 0;
int max_distance = 1000;
int tof1_distance = 0;
int tof2_distance = 0;
int tof3_distance = 0;
int tof4_distance = 0;
int err = 0;
int sr = 0;

void loop() {
  // читаем данные из первого модуля
  if (0 != (tof_1.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
    tof1_distance = tof_1.readReg16Bit(RESULT_RANGE_STATUS + 10);
    tof_1.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
  }

  // читаем данные из второго модуля
  if (0 != (tof_2.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
    tof2_distance = tof_2.readReg16Bit(RESULT_RANGE_STATUS + 10);
    if (tof2_distance >= 8190){
      tof2_distance = 1500;
    }
    tof_2.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
  }

  // читаем данные из третьего модуля
  if (0 != (tof_3.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
    tof3_distance = tof_3.readReg16Bit(RESULT_RANGE_STATUS + 10);
    if (tof3_distance >= 8190){
      tof3_distance = 1500;
    }
    tof_3.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
  }

  // читаем данные из четвертого модуля
  if (0 != (tof_4.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
    tof4_distance = tof_4.readReg16Bit(RESULT_RANGE_STATUS + 10);
    tof_4.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
  }

  dislplay_print_4_tofs(tof1_distance, tof2_distance, tof3_distance, tof4_distance);

  Serial.print(tof1_distance); Serial.print(" ");
  Serial.print(tof2_distance); Serial.print(" ");
  Serial.print(tof3_distance); Serial.print(" ");
  Serial.println(tof4_distance);
  err = (tof3_distance - tof2_distance);
  sr = (tof3_distance + tof2_distance) / 2;
  kp = 0.1;
  motor.write(99);

  if (tof4_distance > max_distance) {
    motor.write(99);
    servo.write(147);
    
  }
  
  if (tof1_distance > max_distance) {
    motor.write(99);
    servo.write(33);
  }


  if ((tof1_distance < max_distance) and (tof4_distance < max_distance)) {
    motor.write(99);
    servo.write(100 + err * kp);
    Serial.println(err * kp);
  }
  if ((sr < 200) or (tof2_distance < 100) or (tof2_distance < 100)){
    servo.write(120);
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
bool setup_4_tofs(int timeout) {
  Serial.println("Подготовка tof-модулей ...");

  // чтобы избежать конфликтов адрессации, деактивируем все модули за исключением одного (с него начнем задавать адреса)
  pinMode(XSHUT_TOF_1, OUTPUT);
  pinMode(XSHUT_TOF_2, OUTPUT);
  pinMode(XSHUT_TOF_3, OUTPUT);
  //  pinMode(XSHUT_TOF_4, OUTPUT);

  Wire.begin();   // активируем интерфейс I2C, arduino будет мастером

  tof_4.setAddress(TOF_4_ADDR);     // задаем адрес для текущего активного модуля (обязательно должен отличаться от 0x29)
  pinMode(XSHUT_TOF_3, INPUT);      // активируем следующий модуль (у него по умолчанию адрес 0х29, поэтому не забываем его сменить)
  delay(10);                        // выжидаем 10 мс для полной активации
  tof_3.setAddress(TOF_3_ADDR);     // меняем адресс (обязательно должен отличаться от 0x29)
  pinMode(XSHUT_TOF_2, INPUT);      // активируем следующий модуль (у него по умолчанию адрес 0х29, поэтому не забываем его сменить)
  delay(10);                        // выжидаем 10 мс для полной активации
  tof_2.setAddress(TOF_2_ADDR);     // меняем адресс (обязательно должен отличаться от 0x29)
  pinMode(XSHUT_TOF_1, INPUT);      // активируем оставшийся датчик и используем его адрес по умолчанию
  delay(10);

  // назначаем адрес первому модулю, активируем и дожидаемся выхода в рабочий режим
  //  tof_1.setAddress(TOF_1_ADDR);
  //  pinMode(XSHUT_TOF_1, INPUT);
  //  delay(10);

  // назначаем адрес второму модулю, активируем и дожидаемся выхода в рабочий режим
  //  tof_2.setAddress(TOF_2_ADDR);
  //  pinMode(XSHUT_TOF_2, INPUT);
  //  delay(10);

  // назначаем адрес третьему модулю, активируем и дожидаемся выхода в рабочий режим
  //  tof_3.setAddress(TOF_3_ADDR);
  //  pinMode(XSHUT_TOF_3, INPUT);
  //  delay(10);

  // назначаем адрес четвертому модулю, активируем и дожидаемся выхода в рабочий режим
  //  tof_4.setAddress(TOF_4_ADDR);
  //  pinMode(XSHUT_TOF_4, INPUT);
  //  delay(10);

  // задаем таймаут ожидания данных для каждого модуля
  tof_1.setTimeout(timeout);
  tof_2.setTimeout(timeout);
  tof_3.setTimeout(timeout);
  tof_4.setTimeout(timeout);

  //инициализируем модули
  if (tof_1.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 1!");
    return 1;
  }

  if (tof_2.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 2!");
    return 1;
  }

  if (tof_3.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 3!");
    return 1;
  }

  if (tof_4.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 4!");
    return 1;
  }

  // запускаем модули в continuius режиме измерений
  tof_1.startContinuous();
  tof_2.startContinuous();
  tof_3.startContinuous();
  tof_4.startContinuous();

  return 0;
}
