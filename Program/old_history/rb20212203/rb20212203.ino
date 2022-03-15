#include <Servo.h>

Servo servo, motor;  // создадим объект сервопривода

void setup() {
  servo.attach(9);  // сервопривод на выводе 9
  motor.attach(10);
}

void loop() {
    servo.write(95);              
    delay(4000);
    servo.write(95);
    delay(4000);
    //motor.write(0);
    servo.write(95);
    //delay(500);
    //motor.write(0);
    delay(4000);
    servo.write(95);
    delay(4000);
}

    
