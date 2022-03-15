#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED display I2C address
#define OLED_ADDR   0x3C
#define OLED_RESET  -1        // no reset pin



//void TCA9548A(uint8_t bus)
//{
// Wire.beginTransmission(0x70); // TCA9548A адрес 0x70
// Wire.write(1 << bus); // отправляем байт на выбранную шину
// Wire.endTransmission();
//}




Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);
void setup() {
    Wire.begin();
    
  TCA9548A(5);
      // initialize and clear display
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    delay(2000);
    display.clearDisplay();
    //display.setRotation(2);
    //display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
//    display.drawPixel(10, 10, SSD1306_WHITE);
//    display.display();
    delay(2000);
//    display.clearDisplay();
    //display.setCursor(0, 0);
    display.print("message");
    display.setCursor(25, 0);
    display.print(6);
    display.display();

}

void loop() {
  display.clearDisplay();
  display.setCursor(25, 0);
  display.print(millis());
  display.display();

}
