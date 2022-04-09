#include <VL53L0X.h>
#include <Wire.h>
#include <Servo.h>
#include <Math.h>
#include <SPI.h>
#include "sensors.h"
#include "define.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

Servo servo, motor;  // создадим объект сервопривода и мотора
Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);

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




bool setup_6_tofs(int timeout) {

  TCA9548A(0);
  Serial.println("Подготовка tof-модулей ...");
  if (tof_0.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 0!");
    return 0;
  }
  tof_0.setTimeout(timeout);
  tof_0.setSignalRateLimit(0.1);
  tof_0.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  tof_0.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  tof_0.startContinuous();

  TCA9548A(1);
  if (tof_1.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 1!");
    return 0;
  }
  tof_1.setTimeout(timeout);
  tof_1.startContinuous();

  TCA9548A(2);
  if (tof_2.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 2!");
    return 0;
  }
  tof_2.setTimeout(timeout);
  tof_2.startContinuous();

  TCA9548A(4);
  if (tof_3.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 3!");
    return 0;
  }
  tof_3.setTimeout(timeout);
  tof_3.setSignalRateLimit(0.1);
  tof_3.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  tof_3.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  tof_3.startContinuous();

  TCA9548A(3);
  if (tof_4.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 4!");
    return 0;
  }
  tof_4.setTimeout(timeout);
  tof_4.startContinuous();


  TCA9548A(7);
  if (tof_5.init() == 0) {
    Serial.println("Не удалось обнаружить и инициализировать tof-модуль 5!");
    return 0;
  }
  tof_5.setTimeout(timeout);
  tof_5.startContinuous();

  Serial.println("success");
  return 0;
}

void s_pos (double deg)
{
  // LEFT +
  //RIGHT -
  servo.write(S0 + deg);
  spos = S0 + deg;
}




//-------------------------------------SETUP------------------------------------------

void setup() {
  Serial.begin(9600);
  Wire.begin();

  servo.attach(PIN_PD4);
  motor.attach(PIN_PB2);
  pinMode(ENCODER_PORT, INPUT);
  pinMode(ANALOG_READ_PORT, INPUT);
  pinMode(INTEGRAL_DATA_1, OUTPUT);
  pinMode(INTEGRAL_DATA_2, OUTPUT);
  pinMode(INTEGRAL_DATA_3, OUTPUT);

  if (setup_6_tofs(1000) != 0) {
    while (1) {}
  }
}


double rpm = 0;
int state = 1;
unsigned long int tt;
int magnet = 0;
int hol = 0;

int velocity_count()
{
  hol = analogRead(ENCODER_PORT);
  if (hol > 640) {
    magnet = 1;
    //Serial.println(1);
  }
  if (hol < 600)
    magnet = 0;

  if (millis() - tt > 500)
    rpm = 0;

  switch (state) {

    case 1:
      if (magnet == 1) {
        tt = millis();
        state = 2;
      }
      break;

    case 2:
      if (magnet == 0)
        state = 3;

      break;

    case 3:
      if (magnet == 1) {
        rpm = 1000.0 / double(millis() - tt);
        tt = millis();
        state = 2;
      }
      break;
  }
}

long int tt_stop = 0;
double kp_sp = 1.5;
//double ki_sp = 0.1;
double kd_sp = 15;
double errold_sp = 0;
//double integral_sp = 0;
int err_sp = 0;
double z = 0;
void speed_control()
{
  
  //integral_sp = 0.5*integral_sp+ki_sp*(sp - rpm);
  err_sp = (kp_sp*(sp - rpm) + kd_sp*(sp - rpm - errold_sp));  
  errold_sp = (sp - rpm);

  
  if (rpm < 4)
    motor.write(102);
  else
    {
      if(err_sp < 0)
      {
        z = NEUTRAL + err_sp/1; //(err_sp * (NEUTRAL-REVERSE) / ((f_sp-t_sp)*kp_sp + (f_sp-t_sp)*kd_sp));
        if (z<REVERSE || rpm > f_f_sp-15 )
          motor.write(REVERSE);
       else
          motor.write(z);
      }
      else
      {
        z = 100 + err_sp/7;//((err_sp *5)/(f_sp*kp_sp + f_sp*kd_sp));
        if(z > 105)
          motor.write(105);
        else
          motor.write(z);
      }
        
    }
  

  if (rpm < 3) {
    tt_stop++;
  }
  else
  {
    if (l_0 < 120 && l_0 > 10 || r_0 < 120 && r_0 > 10)
      tt_stop += 6;
    else
      tt_stop = 0;
  }
}



void tof_read() {

  TCA9548A(0);
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
    if (l_90 > 1200)
      l_90 = 1200;
  }

  TCA9548A(7);
  if (0 != (tof_5.readReg(RESULT_INTERRUPT_STATUS) & 0x07)) {
    r_90 = tof_5.readReg16Bit(RESULT_RANGE_STATUS + 10);
    tof_5.writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
    if (r_90 > 1200)
      r_90 = 1200;
  }

  err_45 = r_45 - l_45;
  err_90 = r_90 - l_90;
  err_0 = (r_0 - l_0)/10;

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
  Serial.print(r_90);
  Serial.print(" ");
  Serial.print(rpm);
  Serial.print(" ");
  Serial.println(sp);
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


int stop_time = 0;
int razvorot = 0;
double kp = 0.011;
double kd = 0.7;
//------------------------------------------------------------------------------------------------------------------------

void loop() {

  s_pos(0);
  delay(3000);


  while (1) {
    velocity_count();
    tof_read();
    speed_control();
    //serial_print();
    //display_print();

    if (razvorot > 50)
      razvorot = 0;

    if (razvorot < -300) {
      s_pos(-30);
      motor.write(NEUTRAL);
      delay(50);
      motor.write(REVERSE);
      delay(150);
      motor.write(NEUTRAL);
      delay(50);
      motor.write(REVERSE + 5);
      delay(1000);
      //while(l_0 < 210 || r_0 < 210)
      motor.write(NEUTRAL);
      tt_stop = -300;
      razvorot = 0;
      s_pos(30);
      motor.write(102);
      delay(1000);
    }


    //------------------------------

    if (tt_stop > 900) {
      s_pos(S0-spos);
      motor.write(NEUTRAL);
      delay(50);
      motor.write(REVERSE);
      delay(150);
      motor.write(NEUTRAL);
      delay(50);
      motor.write(REVERSE + 5);
      delay(700);
      //while(l_0 < 210 || r_0 < 210)
      motor.write(NEUTRAL);
      tt_stop = -300;
    }
    else {


      if ((r_0+l_0)/2 > 1750)
      { // super fast
        sp = f_f_sp;
        s_pos(-(err_45+err_90+err_0)*kp -(err_0+err_45+err_90-err_d)*kd);
        err_d = err_45+err_90+err_0;
      }
      else if ((r_0 > 1100 || l_0 > 1100) && (l_90 > 150 && r_90 > 150) && (l_45 > 160 && r_45 > 160) && (r_90 < 950) && (l_90 < 950)) {
        //forward
          sp = f_sp;
          s_pos(-(err_45+err_90+err_0)*kp -(err_0+err_45+err_90-err_d)*kd);
          err_d = err_45+err_90+err_0;
      } else if (l_90 > 950) {
        // поворачиваем
        //left
        s_pos(30);
        sp = t_sp;
        razvorot--;
      }
      else if (r_90 > 850) {
        //right
        s_pos(-30);
        sp = t_sp;
        razvorot++;
      }
      else {
        //turn
        sp = t_sp;

        if (r_90 > 850) {
          //right
          s_pos(-30);
          sp = t_sp;
          razvorot++;
        }

        else {
          if (l_90 > 950) {
            //left
            s_pos(30);
            sp = t_sp;
            razvorot--;

          }     else if (r_45 > 1000) {
            // light right
            s_pos(-20);
            sp = f_sp;
            razvorot++;
          } else if (l_45 > 1000) {
            //light left
            s_pos(20);
            sp = f_sp;
            //motor.write(NEUTRAL-10);
            razvorot--;
          } else {
            sp = t_sp + 5;
          }


        }

      }
    }






  }

}
