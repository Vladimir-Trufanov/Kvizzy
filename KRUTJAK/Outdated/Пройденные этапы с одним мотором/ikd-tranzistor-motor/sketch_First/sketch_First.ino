// C++ code
//

#include <IRremote.h>

#include <SoftwareSerial.h>
#define pBtn3 3

#define IR_RECEIVE_PIN 2

SoftwareSerial serialTX(5,6);  // RX=5, TX=6
void setup()
{
  pinMode(pBtn3, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  serialTX.begin(300);  

  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);

}

void loop()
{
  // Зажигаем лампочку на полсекунды, когда кнопка нажата
  if (!digitalRead(pBtn3))
  {
    digitalWrite(LED_BUILTIN,1);
    delay(500);
    digitalWrite(LED_BUILTIN,0);

    // Отправляем AT-команды на VirtUART
    serialTX.println("AT+2");    // "поставить на паузу"

    // Отправляем AT-команды на свой RealUART
    serialTX.println("AT+2");    // "поставить на паузу"
    Serial.println("First=3");
  }

if (IrReceiver.decode()) 
   {
      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      uint16_t command = IrReceiver.decodedIRData.command;
      //Serial.print("Vcc0="); Serial.print(analogRead_VCC()); Serial.print("V\r\n");
      //Admin.Steward(Motor,command); 
      Serial.println(command);
      serialTX.print("AT+");    
      serialTX.println(command);   
      delay(200);  
      
      //Serial.print("Vcc1="); Serial.print(analogRead_VCC()); Serial.print("V\r\n");
      // Готовим прием следующего нажатия клавиши
      IrReceiver.resume();
   }
}
