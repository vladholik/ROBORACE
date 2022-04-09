#include <Wire.h>

void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70); // TCA9548A адрес 0x70
  Wire.write(1 << bus); // отправляем байт на выбранную шину
  Wire.endTransmission();
}

int getCamAngle() {
  static int const CHAR_BUF = 10;
  int angle = 0;
  int32_t temp = 0;
  char buff[CHAR_BUF] = {0};

  Wire.requestFrom(0x12, 2);
  if (Wire.available() == 2) { // got length?
    temp = Wire.read() | (Wire.read() << 8);
    delay(1); // Give some setup time...
    Wire.requestFrom(0x12, temp);
    if (Wire.available() == temp) { // got full message?
      temp = 0;
      while (Wire.available()) buff[temp++] = Wire.read();

    } else {
      while (Wire.available()) Wire.read(); // Toss garbage bytes.
    }
  } else {
    while (Wire.available()) Wire.read(); // Toss garbage bytes.
  }
  angle = atoi(buff);
  return angle;
}


void setup() {
  Serial.begin(9600);
  Wire.begin();

}

void loop() {
  TCA9548A(5);
  Serial.println(getCamAngle());
  delay(100);

}
