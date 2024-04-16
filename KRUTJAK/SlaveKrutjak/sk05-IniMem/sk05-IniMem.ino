/** KRUTJAK-SLAVE-PRO_MINI                              *** sk05-IniMem.ino ***
 * 
 * mk05 - Вывести в отдельный модуль переменные и константы 
 *        исполнительной системы паровозика "КРУТЯК"
 * 
 * v3.0, 16.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

#include <SoftwareSerial.h>

#include "define_slave_kru.h"  // подключили определения управляющей системы 

void setup() 
{
   serialSlave.begin(300);  
   Serial.begin(9600);
}

void loop() 
{
   while(serialSlave.available())
   {
      simb=serialSlave.read();
      strData += (char)simb;        // забиваем строку принятыми данными
      //String str(simb);
      //Serial.print(str);
      //strData = strData+str;        // забиваем строку принятыми данными
      recievedFlag = true;                   // поднять флаг что получили данные
      delay(40);                     // ЗАДЕРЖКА ожидания очистки буфера после символа !!!
   }

   if (recievedFlag) 
   {                      // если данные получены
      //Serial.println("strData:");               // вывести
      Serial.println(strData);               // вывести
      strData = "";                          // очистить
      recievedFlag = false;                  // опустить флаг
   }

  
   /*
   if (serialSlave.available())
   {
      //uint16_t com=getCom(serialRX);
      //Serial.println(com);
      
      //while(serialRX.available())
      //{
         simb=serialSlave.read();
         strData += (char)simb;        // забиваем строку принятыми данными
         String str(simb);
         Serial.print(str);
      //}

   }
   */
}

/*
  Данный код позволяет принять данные, идущие из порта, в строку (String) без "обрывов"
*/
// Принять команду
uint16_t getCom(SoftwareSerial serialSlave) 
{
   while (serialSlave.available() > 0) 
   {         // ПОКА есть что то на вход    
      strData += (char)serialSlave.read();        // забиваем строку принятыми данными
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

// ******************************************************** sk05-IniMem.ino ***
