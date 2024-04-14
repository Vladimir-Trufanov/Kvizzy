/** KRUTJAK-ESP01 ************************************** *** mk01-Buzer.ino ***
 * 
 * 1 - По клавише "System" запустить тестирование системы (пока три пика зумера)
 * 
 * v1.1, 14.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

#include <IRremote.hpp>
#define IR_RECEIVE_PIN 2
#define buzPin 0

bool isFirst = false;

void setup() 
{
   Serial.begin(115200);
   pinMode(buzPin, OUTPUT);  
   digitalWrite(buzPin, LOW);
   IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
}

void loop() 
{
   if (isFirst==false)
   {
      delay(5000);  
      Serial.print("-"); delay(500);  
      Serial.print("-"); delay(500);  
      Serial.print("-"); delay(500);  
      Serial.print("-"); delay(500);  
      Serial.println("-");  
      Serial.println("КРУТЯК-Мастер подключен");
      Serial.println("Прием ИК-сигналов запущен");
      isFirst=true;
   }
   
   if (IrReceiver.decode()) 
   {
      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      uint16_t command = IrReceiver.decodedIRData.command;
      if (command==50)
      {
         Serial.println("до");
         digitalWrite(buzPin, HIGH);
         delay(100);  
         digitalWrite(buzPin, LOW);
         delay(100);  
         digitalWrite(buzPin, HIGH);
         delay(100);  
         digitalWrite(buzPin, LOW);
         delay(100);  
         digitalWrite(buzPin, HIGH);
         delay(100);  
         digitalWrite(buzPin, LOW);
         Serial.println("после");
      }
      Serial.println(command);
      // Делаем задержку в 100 мс, чтобы отработать сигнал от нажатия клавиши 
      delay(100);  
      // Готовим прием следующего нажатия клавиши
      IrReceiver.resume();
   }
}

// ********************************************************* mk01-Buzer.ino ***
