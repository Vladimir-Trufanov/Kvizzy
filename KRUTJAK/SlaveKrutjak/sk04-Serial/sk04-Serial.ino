/** KRUTJAK-SLAVE-PRO_MINI ****************************** *** sk04-Serial.ino ***
 * 
 * 1 - По клавише "System" запустить тестирование системы (пока три пика зумера)
 * 
 * v1.1, 14.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

#include <SoftwareSerial.h>
SoftwareSerial serialRX(4,3);   // RX=4, TX=3

volatile char simb;
bool isTrass=true; // true - трассировать принимаемые команды 

uint16_t command = 0; // последняя полученная команда

String strData = "";
boolean recievedFlag;



void setup() 
{
   serialRX.begin(300);  
   Serial.begin(9600);
}

void loop() 
{
   if (serialRX.available())
   {
      //uint16_t com=getCom(serialRX);
      //Serial.println(com);
      
      //while(serialRX.available())
      //{
         simb=serialRX.read();
         String str(simb);
         Serial.print(str);
      //}

   }
}

/*
  Данный код позволяет принять данные, идущие из порта, в строку (String) без "обрывов"
*/
// Принять команду
uint16_t getCom(SoftwareSerial serialRX) 
{
   while (serialRX.available() > 0) 
   {         // ПОКА есть что то на вход    
      strData += (char)serialRX.read();        // забиваем строку принятыми данными
      recievedFlag = true;                   // поднять флаг что получили данные
      delay(2);                              // ЗАДЕРЖКА. Без неё работает некорректно!
   }

   if (recievedFlag) 
   {                      // если данные получены
      Serial.println(strData);               // вывести
      strData = "";                          // очистить
      recievedFlag = false;                  // опустить флаг
   }
   return 4; 
}

// ********************************************************* sk04-Serial.ino ***
