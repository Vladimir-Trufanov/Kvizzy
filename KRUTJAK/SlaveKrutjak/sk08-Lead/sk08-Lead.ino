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
   
   // Задаем начальное направление движения
   motor_direct(forward);   
   // Задаем начальную скорость
   motor_speed(0);

   // Инициируем секундное первое прерывание (с частотой в 1 Гц)
   IniTimer2();
   pinMode(LEDPIN, OUTPUT);
}

void loop() 
{
   // Контроллируем скорость на постоянном SHIM
   analogWrite(PWM_PIN,currShim);

   // Снимаем напряжение батареи
   VccSlave=analogRead_VCC();
   // Определяем напряжение на контакте мотора
   //    VccSlave --> 255
   //    U        --> currShim
   float  U = VccSlave * currShim / 255;
   // Определяем мощность на контакте мотора: P=U*U/R, где R = 13 Ом
   PwrSlave = U*U / 13;
   // Готовим данные для передачи в управляющую систему  
   sVcc  = String(VccSlave,2);        // напряжение питания 
   sPwr  = String(PwrSlave,2);        // мощность на контакте 
   if (currDir==forward) sDir="+"; else sDir="-";
   sShim=String(currShim);
   if (currShim<100) sShim="0"+sShim;
   if (currShim<10)  sShim="0"+sShim; // ШИМ на контакте  

   //motion_to_max(forward);
   //motion_to_max(back);
   
   
   
  
   /*
   analogWrite(PWM_PIN, currShim);
   */

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
      
   if (OneSecondFlag==true)
   {
      // Меняем состояние контрольного светодиода здесь в основном цикле
      // для того, чтобы видеть как часто он запускается
      doBurns=!doBurns;
      digitalWrite(LEDPIN,doBurns);
      
      // Формируем и возвращаем контрольную информацию управляющей системе
      strInfo=sDir+sShim+": "+sPwr;
      //serialSlave.print("=== "+command+" ===");
      serialSlave.print(strInfo);
      delay(40); // выдержали паузу, чтобы команда спокойно ушла

      // Сбрасываем флаг одной секунды
      OneSecondFlag = false;
   }

}
// ****************************************************************************
// *           Отработать команду по оборудованию исполняющей системы         *
// ****************************************************************************

/** Система команд ИК-пульта управления паровозиком
 * 
 *  1 - "стоп паровоз"       	- TV-VCR
 *  2 -                 		  - ON standby
 *  3 - "начать движение"    	- A.REP 
 * 20 - "начать движение"    	- REC 
 * 21 - "назад"              	- == 
 * 22 -                      	- >|<    
 * 24 - "медленнее"          	- <<  
 * 25 - "вперед"             	- =>
 * 26 - "быстрее"            	- >>   
 * 32 - "звук вкл/выкл"      	- EJECT
 * 33 - "стоп паровоз"       	- A.TRK
 * 44 - "подключить"         	- TRACKING ON 
 * 45 - "отключить"          	- TRACKING OFF 
 * 50 - "тестировать систему"	- SYSTEM
 * 
**/

int actionCom(String command) 
{
   int reskom=0;   // "действие выполнено успешно"
   
   // "начать движение"
   if (command=="03")
   {
      motor_speed(MAX_SPEED);
      motor_speed(START_SPEED);
   }
   if (command=="20")
   {
      motor_speed(MAX_SPEED);
      motor_speed(START_SPEED);
   }

   // "стоп паровоз"
   if (command=="01")
   {
      motor_speed(0);
   }
   if (command=="33")
   {
      motor_speed(0);
   }

   // "медленнее"   
   if (command=="24")
   {
      // Реагируем в предположении, что мотор не работал
      if (currShim<MIN_SPEED) 
      {
         motor_speed(MAX_SPEED);
         motor_speed(MIN_SPEED);
      }
      // Реагируем, когда мотор работает
      else
      {
         if (currShim-DELTA_SPEED<MIN_SPEED) currShim=MIN_SPEED;
         else currShim=currShim-DELTA_SPEED;
      }
   }
   // "быстрее" 
   if (command=="26")
   {
      // Реагируем в предположении, что мотор не работал
      if (currShim+DELTA_SPEED<MIN_SPEED) 
      {
         motor_speed(MAX_SPEED);
         motor_speed(MIN_SPEED);
      }
      else
      {
         if (currShim+DELTA_SPEED>MAX_SPEED) currShim=MAX_SPEED;
         else currShim=currShim+DELTA_SPEED;
      }
   }

   // "назад" - если двигались вперед, то останавливаем движение 
   // и начинаем движение назад
   if (command=="21")
   {
      if (currDir==forward)
      {
         motor_speed(0);
         motor_direct(back);   
         motor_speed(MAX_SPEED);
         motor_speed(MIN_SPEED);
      }
   }

   // "вперед" - если двигались назад, то останавливаем движение 
   // и начинаем движение вперед
   if (command=="25")
   {
      if (currDir==back)
      {
         motor_speed(0);
         motor_direct(forward);   
         motor_speed(MAX_SPEED);
         motor_speed(MIN_SPEED);
      }
   }

   command=="00";
   return reskom;
}

// ********************************************************** sk08-Lead.ino ***
