#include "U8g2lib.h"
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

boolean LEDF = true;
boolean FOTO = true;

void setup() {
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvR10_tr);
  u8g2.sendBuffer();

  pinMode(13, OUTPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(3, OUTPUT); //LED

  pinMode(2, INPUT);
  Serial.begin(115200);
}

void loop() {
  //кнопки
  if (!digitalRead(14)) LEDF = true;
  if (!digitalRead(15)) LEDF = false;
  if (!digitalRead(16)) FOTO = true;
  if (!digitalRead(17)) FOTO = false;

  digitalWrite(13, digitalRead(2));

  if (LEDF == true) {
    digitalWrite(3, digitalRead(2));
  }

  if ((digitalRead(2)) && (FOTO == true)) {
    Serial.print('#');
    FOTO = false;
  }

  if (Serial.available() > 0) {  //если есть доступные данные
    // считываем байт
    char incomingByte = Serial.read();
    if (incomingByte == '@') {
      FOTO = true;
    }
  }

  u8g2.clearBuffer();
  u8g2.setCursor(0, 12);
  u8g2.print("LEDF ");
  u8g2.print(LEDF);
  u8g2.setCursor(0, 26);
  u8g2.print("FOTO ");
  u8g2.print(FOTO);
  u8g2.sendBuffer();
}
