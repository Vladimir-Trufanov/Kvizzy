/** KRUTJAK-SLAVE-PRO_MINI                                     *** sk33.ino ***
 * 
 * sk33 - Исполнительная система паровозика "КРУТЯК"
  * 
 * v3.5, 10.05.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

#include <iarduino_VCC.h> 
#include <SoftwareSerial.h>

#include "timer2_kru.h"        // подключили 1-ое таймерное прерывание 
#include "define_slave_kru.h"  // подключили определения исполнительной системы 
#include "common_slave_kru.h"  // подключили общие функции исполнительной системы 

/** Примерно за 1 секунду управляющей системе ( Master) отправляются несколько 
 * сообщений разных типов через одинаковые интервалы:
 *  
 *  "+" - ШИМ и мощность на контакте двигателя при движении вперед
 *  "-" - ШИМ и мощность на контакте двигателя при движении назад
 *  "~" - напряжение питания исполнительной системы
 *  "*" - Диагностическое сообщение
 */

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

void loopi() 
{
   currDir=0;
   digitalWrite(DIR_PIN,currDir);
   
   analogWrite(PWM_PIN,155);
   delay(4000);              
   analogWrite(PWM_PIN,0);
   delay(4000);              
   
   currDir=1;
   digitalWrite(DIR_PIN,currDir);

   analogWrite(PWM_PIN,155);
   delay(4000);              
   analogWrite(PWM_PIN,0);
   delay(4000);              
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
   if (ModeSlave==modeDebug) currShim=115;
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
      currCmd = strData.substring(0,5);
      command = strData.substring(3,5);
      // Отрабатываем команду по оборудованию исполняющей системы 
      reskom=actionCom(command); 
      // Чистим командную последовательность и сбрасываем флаг
      strData = "";                     
      recievedFlag = false;           
   }
   // Отправляем информационное сообщение управляющей системе   
   if (Motor1_Flag == true)
   {
      Motor1_Flag = false;
      strInfo=sDir+sShim+" p"+sPwr+sDir;
      serialSlave.print(strInfo);
      if (ModeSlave==modeDebug) Serial.println(strInfo);
   } 
   // Отправляем информационное сообщение управляющей системе   
   else if (Vcc2_Flag == true)
   {
      Vcc2_Flag = false;
      strInfo="Vcc"+sVcc+"~";
      serialSlave.print(strInfo);
      if (ModeSlave==modeDebug) Serial.println(strInfo);
   } 
   // Отправляем информационное сообщение управляющей системе   
   else if (Act3_Flag == true)
   {
      Act3_Flag = false;
      if (currCmd != oldiCmd)
      {
        oldiCmd=currCmd; 
        strInfo=currCmd+"!";
        serialSlave.print(strInfo);
        if (ModeSlave==modeDebug) Serial.println(strInfo);
      }
   }
   // Выдерживаем паузу, чтобы команда спокойно ушла
   delay(4); 
    
   // Меняем состояние лампочки после почти секунды
   if (OneSecondFlag==true)
   {
      // Сбрасываем флаг одной секунды
      OneSecondFlag = false;
      // Меняем состояние контрольного светодиода здесь в основном цикле
      // для того, чтобы видеть как часто он запускается
      doBurns=!doBurns;
      digitalWrite(LEDPIN,doBurns);
      if (ModeSlave==modeDebug) Serial.println("Лампочка");
   }
}

// *************************************************************** sk32.ino ***
