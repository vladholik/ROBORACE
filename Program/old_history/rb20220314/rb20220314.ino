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


Servo servo, motor;  // создадим объект сервопривода и мотора
Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);



double spos = 0;
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
int err_0 = 0;
int err_d = 0;
int encoder = 0;
double sp = 0;
double t_sp = 22;
double f_sp = 25;
double f_f_sp = 45;




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
        if (z<REVERSE)
          motor.write(REVERSE);
       else
          motor.write(z);
      }
      else
      {
        z = 100 + err_sp/6;//((err_sp *5)/(f_sp*kp_sp + f_sp*kd_sp));
        if(z > 108)
          motor.write(108);
        else
          motor.write(z);
      }
        
    }
  

  if (rpm < 2) {
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


err_45 = r_45 - l_45;
  err_90 = r_90 - l_90;
  err_0 = (r_0 - l_0)/10;
  
void loop() {

  s_pos(0);
  delay(3000);


  while (1) {
    velocity_count();
    tof_read();
    speed_control();
    //serial_print();
    //display_print();

    if (razvorot > 70)
      razvorot = 0;

    if (razvorot < -450) {
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


      if ((r_0+l_0)/2 > 1650)
      { // super fast
        sp = f_f_sp;
        s_pos(-(err_45+err_90)*kp -(err_45+err_90-err_d)*kd);
        err_d = err_45+err_90;
      }
      else if ((r_0 > 1300 || l_0 > 1300) && (l_90 > 150 && r_90 > 150) && (l_45 > 160 && r_45 > 160) ) {
        //forward
          sp = f_sp;
          s_pos(-(err_45+err_90)*kp -(err_45+err_90-err_d)*kd);
          err_d = err_45+err_90;
      } else if (l_90 > 950) {
        // поворачиваем
        //left
        s_pos(30);
        sp = t_sp;
        razvorot--;
      }
      else if (r_90 > 950) {
        //right
        s_pos(-30);
        sp = t_sp;
        razvorot++;
      }
      else {
        //turn
        sp = t_sp;

        if (r_90 > 950) {
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
