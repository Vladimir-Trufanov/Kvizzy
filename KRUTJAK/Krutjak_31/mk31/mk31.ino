/** KRUTJAK-MASTER-PRO_MINI                                    *** mk31.ino ***
 * 
 * mk31 - Управляющая система паровозика "КРУТЯК" 
 * 
 * v3.3, 27.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

// ИСКР - исполнительная система паровозика "КРУТЯК"
// УПС  - управляющая система

/** Примерно за 1 секунду от исполнительной системы приходят несколько сообщенияй 
 *  разных типов через одинаковые интервалы:
 *  
 *  "+" - ШИМ и мощность на контакте двигателя при движении вперед
 *  "-" - ШИМ и мощность на контакте двигателя при движении назад
 *  "~" - напряжение питания исполнительной системы
 *  "*" - Диагностическое сообщение
 */

#include <iarduino_VCC.h> 
#include <iarduino_OLED_txt.h>                         
#include <IRremote.hpp>
#include <SoftwareSerial.h>

#include "define_master_kru.h"     // подключили определения управляющей системы 
#include "common_master_kru.h"     // подключили общие функции управляющей системы 
#include "timer1_kru.h"            // подключили 1-ое таймерное прерывание  

void setup() 
{
   myOLED.begin();                 // инициировали работу с дисплеем.
   myOLED.setFont(MediumFontRus);  // указали шрифт для вывода чисел и текста.

   serialMaster.begin(2400);  

   pinMode(buzPin, OUTPUT);  
   digitalWrite(buzPin, LOW);
   IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);

   pinMode(LEDPIN, OUTPUT);
   
   // Инициируем четветрь секундное прерывание (с частотой в 1/4 Гц)
   // (для управляющей системы, чуть пореже, чтобы таймеры систем 
   // расходились, но медленно)
   IniTimer1(3906);  // 1 четверть => 15624/4=3906
   if (ModeSlave==modeDebug) Serial.begin(9600);
}

void loop() 
{
   if (isFirst==false)
   {
      // Делаем задержку и трассируем включение
      delay(1000);  
      buzz_Ok();
      myOLED.clrScr(); // почистили экран
      isFirst=true;
      if (ModeSlave==modeDebug) sendTrass(F("FirstLoop"));
   }
   
   // Принимаем и собираем командную последовательность
   // от управляющей системы в строку (String) без "обрывов"
   while(serialMaster.available())
   {
      ysimb=serialMaster.read();
      ystrData += (char)ysimb;          // добавили в строку принятый символ
      yrecievedFlag = true;             // устанавливаем флаг, что получили данные
      delay(4);                        // ожидание завершения поступления символов !!!
   }
   // Разбираем команду и выполняем действие
   if (yrecievedFlag) 
   {
      // Запоминаем принятое сообщение и его тип
      typeData=ysimb;
      viewData=ystrData;                           
      // Готовим наполнение нового сообщения и сбрасываем флаг
      ystrData = "";                     
      yrecievedFlag = false;  
      myOLED.print(typeData,50,7);
   }
   VccSlave=4.95;
   // Регистрируем поступление команды от инфракрасного датчика
   if (IrReceiver.decode()) 
   {
      // myOLED.clrScr(); // почистили экран
      if (ModeSlave==modeDebug) markIKS(); 
      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      command = IrReceiver.decodedIRData.command;
      // Отправляем команду в Slave по VirtUART
      if (command!=0)
      {
         sCommand = String(command);
         if (command<10) sCommand="0"+sCommand;
         serialMaster.print("AT+"+sCommand+".");
         // Делаем задержку в 100 мс, чтобы отработать сигнал от нажатия клавиши 
         delay(100);  
      }
      //
      if (command==50)
      {
         buzz_Ok();
      }
      // Готовим прием следующего нажатия клавиши
      IrReceiver.resume();
   }
   // Выполняем действия по прошествии 1/4 секунды
   if (OneQuatrFlag==true)
   {
      // Сбрасываем флаг одной секунды
      OneQuatrFlag = false;
      // Выводим информацию на дисплей
      //viewState();
      // Меняем состояние контрольного светодиода
      quatr=quatr+1;
      if (quatr==4)
      {
         quatr=0;
         doBurns=!doBurns;
         digitalWrite(LEDPIN,doBurns);
      }

         cli();
         myOLED.print(viewData,5,4); 
         myOLED.print(quatr,5,7);
         sei(); 

   }
}

void buzz_Ok()
{
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
}

// ****************************************************************************
// *     Вывести трассировочное сообщение в com-порт и в 0 строку дисплея     *
// ****************************************************************************
void sendTrass(String message)
{
   Serial.println(message); 
   myOLED.print(message,0,1); 
}
// ****************************************************************************
// *               Отметить в режиме отладки прием ИК-сигнала                 *
// ****************************************************************************
void markIKS()
{
   sendTrass(F("ИК сигнал"));
   buzz_Ok();
   delay(1000);  
   sendTrass(F("         "));
}

// *************************************************************** mk31.ino ***
