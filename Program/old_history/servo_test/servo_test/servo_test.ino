#include <Servo.h>

#define TEST_PIN PIN_PD5
 
Servo servo, motor; 

void setup() {
  Serial.begin(9600);
//  pinMode(PIN_PD5, OUTPUT);
  servo.attach(TEST_PIN);  
  //motor.attach(PIN_PB2);
}

void loop() {
  servo.write(70);
 // motor.write(100);
 delay(2000);
 servo.write(130);
 delay(2000);
  //Serial.println(100);
//  digitalWrite(PIN_PD5, HIGH);
//  delay(20);
//  digitalWrite(PIN_PD5, LOW);
//  delay(20);
 }
