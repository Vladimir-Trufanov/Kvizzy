/** KRUTJAK-MASTER-PRO_MINI                            *** mk07-reciVCC.ino ***
 * 
 * mk07 - Принять от исполнительной системы напряжение батареи 
 *        и потенциал драйвера мотора
 * 
 * v3.2, 18.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

#include <iarduino_VCC.h> 
#include <iarduino_OLED_txt.h>                         
#include <IRremote.hpp>
#include <SoftwareSerial.h>

#include "define_master_kru.h"     // подключили определения управляющей системы 
#include "common_master_kru.h"     // подключили общие функции управляющей системы 
#include "timer_kru.h"             // подключили 1-ое таймерное прерывание  

void setup() 
{
   myOLED.begin();                 // инициировали работу с дисплеем.
   myOLED.setFont(MediumFontRus);  // указали шрифт для вывода чисел и текста.

   serialMaster.begin(2400);  

   pinMode(buzPin, OUTPUT);  
   digitalWrite(buzPin, LOW);
   IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);

   pinMode(LEDPIN, OUTPUT);
   
   // Инициируем секундное первое прерывание (с частотой в 1 Гц)
   // (для управляющей системы, чуть пореже, чтобы таймеры систем 
   // расходились, но медленно)
   IniTimer1(15627);
}

void loop() 
{
   if (isFirst==false)
   {
      // Подключаем блок питания SLAVE
      // Делаем задержку и трассируем включение
      delay(1000);  
      buzz_Ok();
      myOLED.clrScr(); // почистили экран
      isFirst=true;
   }
   
   // Принимаем и собираем командную последовательность
   // от управляющей системы в строку (String) без "обрывов"
   while(serialMaster.available())
   {
      char ysimb=serialMaster.read();
      ystrData += (char)ysimb;          // добавили в строку принятый символ
      yrecievedFlag = true;             // устанавливаем флаг, что получили данные
      delay(40);                        // ожидание завершения поступления символов !!!
   }
   // Разбираем команду и выполняем действие
   if (yrecievedFlag) 
   {
      viewData=ystrData;                           
           
      // Чистим командную последовательность и сбрасываем флаг
      ystrData = "";                     
      yrecievedFlag = false;           
   }

   
   VccSlave=4.90;

   if (IrReceiver.decode()) 
   {
      // myOLED.clrScr(); // почистили экран

      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      command = IrReceiver.decodedIRData.command;
      // Отправляем команду в Slave по VirtUART
      if (command!=0)
      {
         sCommand = String(command);
         if (command<10) sCommand="0"+sCommand;
         serialMaster.println("AT+"+sCommand+".");
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
   
   // Выполняем действия по прошествии 1 секунды
   if (OneSecondFlag==true)
   {
      // Меняем состояние контрольного светодиода
      doBurns=!doBurns;
      digitalWrite(LEDPIN,doBurns);
      // Выводим информацию на монитор
      viewState(sCommand,VccSlave);
      // Сбрасываем флаг одной секунды
      OneSecondFlag = false;
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

/*
void yield() 
{
   // Принимаем и собираем командную последовательность
   // от управляющей системы в строку (String) без "обрывов"
   while(serialMaster.available())
   {
      String ystrData;   
      char ysimb=serialMaster.read();
      ystrData += (char)ysimb;   // добавили в строку принятый символ
      bool yrecievedFlag = true;        // устанавливаем флаг, что получили данные
      delay(40);                        // ожидание завершения поступления символов !!!
   }
}
*/
// ******************************************************* mk07-reciVCC.ino ***
