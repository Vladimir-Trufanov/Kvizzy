// C++ code
//
#include <SoftwareSerial.h>
#define pBtn2 2

SoftwareSerial serialTX(5,6);  // RX=5, TX=6
void setup()
{
  pinMode(pBtn2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  serialTX.begin(300);  
}

void loop()
{
  // Зажигаем лампочку на полсекунды, когда кнопка нажата
  if (!digitalRead(2))
  {
    digitalWrite(LED_BUILTIN,1);
    delay(500);
    digitalWrite(LED_BUILTIN,0);
    // Отправляем AT-команды на свой RealUART
    Serial.println("AT+PAUS");      // "поставить на паузу"
    Serial.println("AT+FRE=980");   // "включить Вести FM на частоте 98МГц"
    Serial.println("First=3");

    // Отправляем AT-команды на VirtUART
    serialTX.println("AT+PAUS");    // "поставить на паузу"
    serialTX.println("AT+FRE=980"); // "включить Вести FM на частоте 98МГц"
  }

  /*
  Serial.println  ("Time="+String(millis())+' '+
                 "Status="+String(!digitalRead(2)));
  serialTX.println("Time="+String(millis())+' '+
                 "Status="+String(!digitalRead(8)));
  */
  //delay(10);
}
