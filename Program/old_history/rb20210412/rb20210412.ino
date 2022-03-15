#include <VL53L0X.h>  
#include <Wire.h>     

#define SYSTEM_INTERRUPT_CLEAR       0x0B
#define RESULT_INTERRUPT_STATUS      0x13
#define RESULT_RANGE_STATUS          0x14

#define XSHUT_TOF_1  1
#define XSHUT_TOF_2  2
#define XSHUT_TOF_3  3
#define XSHUT_TOF_4  4

#define TOF_1_ADDR  0x2A
#define TOF_2_ADDR  0x2B
#define TOF_3_ADDR  0x2C
#define TOF_4_ADDR  0x2D

VL53L0X tof_1;
VL53L0X tof_2;
VL53L0X tof_3;
VL53L0X tof_4;

bool setup_4_tofs(int timeout);


void setup() { 
  Serial.begin(9600);
  while (!Serial) {
    ; // ожидаем подключения монитора порта
  }
  
  if (setup_4_tofs(1000) != 0) {
    while (1) {}
  }

  Serial.println("Arduino setup has been successfully finished!");
}


int tof1_distance = 0;
int tof2_distance = 0;
int tof3_distance = 0;
int tof4_distance = 0;

void loop() {
    // читаем данные из первого модуля
    if (0 != (tof_1.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        tof1_distance = tof_1.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_1.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
    }

    // читаем данные из второго модуля
    if (0 != (tof_2.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        tof2_distance = tof_2.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_2.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
    }

    // читаем данные из третьего модуля
    if (0 != (tof_3.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        tof3_distance = tof_3.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_3.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
    }

    // читаем данные из четвертого модуля
    if (0 != (tof_4.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
        tof4_distance = tof_4.readReg16Bit(RESULT_RANGE_STATUS + 10);
        tof_4.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
    }
    
    //Serial.print(tof1_distance); Serial.print(" ");
    //Serial.print(tof2_distance); Serial.print(" ");
    Serial.print(tof3_distance); Serial.println(" ");
    //Serial.println(tof4_distance);
}

bool setup_4_tofs(int timeout) {
  Serial.println("Подготовка tof-модулей ...");
  
  // деактивируем все модули, чтобы избежать конфликтов адрессации
  pinMode(XSHUT_TOF_1, OUTPUT);
  pinMode(XSHUT_TOF_2, OUTPUT);
  pinMode(XSHUT_TOF_3, OUTPUT);
  pinMode(XSHUT_TOF_4, OUTPUT);

  
  Wire.begin();   // активируем интерфейс I2C, arduino будет мастером

  // назначаем адрес первому модулю, активируем и дожидаемся выхода в рабочий режим 
  tof_1.setAddress(TOF_1_ADDR);

  pinMode(XSHUT_TOF_1, INPUT);
  delay(10);
  // назначаем адрес второму модулю, активируем и дожидаемся выхода в рабочий режим
  tof_2.setAddress(TOF_2_ADDR);
  pinMode(XSHUT_TOF_2, INPUT);
  delay(10);

  // назначаем адрес третьему модулю, активируем и дожидаемся выхода в рабочий режим
  tof_3.setAddress(TOF_3_ADDR);
  pinMode(XSHUT_TOF_3, INPUT);
  delay(10);
  
  // назначаем адрес четвертому модулю, активируем и дожидаемся выхода в рабочий режим
  tof_4.setAddress(TOF_4_ADDR);
  pinMode(XSHUT_TOF_4, INPUT);
  delay(10);

  // задаем таймаут ожидания данных для каждого модуля
  tof_1.setTimeout(timeout);
  tof_2.setTimeout(timeout);
  tof_3.setTimeout(timeout);
  tof_4.setTimeout(timeout);

  // инициализируем модули
  //if (tof_1.init() == 0){
  //  Serial.println("Не удалось обнаружить и инициализировать tof-модуль 1!");
  //  return 1;
  //}
  
  //if (tof_2.init() == 0) {
  //  Serial.println("Не удалось обнаружить и инициализировать tof-модуль 2!");
  //  return 1;
  //}
  
  if (tof_3.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 3!");
    return 1;
  }

  //if (tof_4.init() == 0) {
  //  Serial.println("Не удалось обнаружить и инициализировать tof-модуль 4!");
  //  return 1;
  //}

  // запускаем модули в continuius режиме измерений
  tof_1.startContinuous();
  tof_2.startContinuous();
  tof_3.startContinuous();
  tof_4.startContinuous();  

  return 0;
}
