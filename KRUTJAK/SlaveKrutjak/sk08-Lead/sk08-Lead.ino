/** KRUTJAK-SLAVE-PRO_MINI                                *** sk08-Lead.ino ***
 * 
 * sk08 - Отработать команды управляющей системы 
 * 
 * v3.4, 21.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

#include <iarduino_VCC.h> 
#include <SoftwareSerial.h>

#include "timer2_kru.h"        // подключили 1-ое таймерное прерывание 
#include "define_slave_kru.h"  // подключили определения исполнительной системы 
#include "common_slave_kru.h"  // подключили общие функции исполнительной системы 

void setup() 
{
   serialSlave.begin(2400); 
   if (ModeSlave==modeDebug) Serial.begin(9600);
   
   // Подключаем выводы мотора: направление и мощность
   pinMode(DIR_PIN, OUTPUT);
   pinMode(PWM_PIN, OUTPUT);

   // Инициируем секундное первое прерывание (с частотой в 1 Гц)
   IniTimer2();
   pinMode(LEDPIN, OUTPUT);
}

/** Система команд ИК-пульта управления паровозиком
 * 
 * - 1 - "стоп паровоз"       	- TV-VCR
 * - 2 - "включить SLAVE" 		   - ON standby
 * - 3 - "начать движение"    	- A.REP 
 * -20 - "начать движение"    	- REC 
 * -21 - "назад"              	- == 
 * -22 -                      	- >|<    
 * -24 - "медленнее"          	- <<  
 * -25 - "вперед"             	- =>
 * -26 - "быстрее"            	- >>   
 * -32 - "звук вкл/выкл"      	- EJECT
 * -33 -                      	- A.TRK
 * -44 - "подключить"         	- TRACKING ON 
 * -45 - "отключить"          	- TRACKING OFF 
 * -50 - "тестировать систему"	- SYSTEM
 * 
**/

void loop() 
{
   // Снимаем напряжение батареи
   VccSlave=analogRead_VCC();
   // Определяем напряжение на контакте мотора
   //    VccSlave --> 255
   //    U        --> currShim
   float  U = VccSlave * currShim / 255;
   // Определяем мощность на контакте мотора: P=U*U/R, где R = 13 Ом
   PwrSlave = U*U / 13;
   // Готовим данные для передачи в управляющую систему  
   sVcc  = String(VccSlave,2);     // напряжение питания 
   sPwr  = String(PwrSlave,2);     // мощность на контакте 
   if (currDir==forward) sDir="+"; else sDir="-";
   sShim = String(currShim);       // ШИМ на контакте     


   /*
   digitalWrite(DIR_PIN, HIGH);
   move();
   digitalWrite(DIR_PIN, LOW);
   move();
   */





   
   motion_to_max(forward);
   motion_to_max(back);
   
   if (OneSecondFlag==true)
   {
      // Меняем состояние контрольного светодиода
      doBurns=!doBurns;
      digitalWrite(LEDPIN,doBurns);
      serialSlave.print(strInfo);
      delay(40); // выдержали паузу, чтобы команда спокойно ушла
      // Сбрасываем флаг одной секунды
      OneSecondFlag = false;
   }
   
   
   
  
   /*
   analogWrite(PWM_PIN, currShim);

   // Принимаем и собираем командную последовательность
   // от управляющей системы в строку (String) без "обрывов"
   while(serialSlave.available())
   {
      simb=serialSlave.read();
      strData += (char)simb;   // добавили в строку принятый символ
      recievedFlag = true;     // установили флаг, что получили данные
      delay(40);               // ожидание завершения поступления символов !!!
   }
   // Разбираем команду и выполняем действие
   if (recievedFlag) 
   {                      
      if (ModeSlave==modeDebug) Serial.println(strData);  
      // Извлекаем код команды
      command = strData.substring(3,5);
      // Отрабатываем команду по оборудованию исполняющей системы 
      reskom=actionCom(command);      
      // Чистим командную последовательность и сбрасываем флаг
      strData = "";                     
      recievedFlag = false;           
   }
   */
}
// ****************************************************************************
// *           Отработать команду по оборудованию исполняющей системы         *
// ****************************************************************************
int actionCom(String command) 
{
   int reskom=0;   // "действие выполнено успешно"
   serialSlave.print("=== "+command+" ===");
   delay(40);

   if (command=="03")
   {
      currShim=254; //MAX_SPEED;
      analogWrite(PWM_PIN, currShim);
      //delay(40);
      //currShim=250;
      //analogWrite(PWM_PIN, currShim);
      
      //analogWrite(PWM_PIN, MAX_SPEED);
   }
   if (command=="01")
   {
      currShim=0;
      //analogWrite(PWM_PIN, currShim);
   }
   if (command=="24")
   {
      currShim=254; //currShim-10;
      analogWrite(PWM_PIN, currShim);
      delay(4000);
   }
   command=="00";
   return reskom;
}


/*
// ****************************************************************************
// *      Выполнить передачу состояния системы в независимом 2 процессе       *
// ****************************************************************************
void yield() 
{
   if (OneSecondFlag==true)
   {
      //serialSlave.print("strInfoYield");
      //delay(40); // выдержали паузу, чтобы команда спокойно ушла
      // Сбрасываем флаг одной секунды
      OneSecondFlag = false;
   }
}
*/

// ********************************************************** sk08-Lead.ino ***
