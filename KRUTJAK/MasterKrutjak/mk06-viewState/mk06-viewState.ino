/** KRUTJAK-MASTER-PRO_MINI                          *** mk06-viewState.ino ***
 * 
 * mk06 - Обеспечить вывод информации на дисплей 
 * 
 * v3.1, 17.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

#include <iarduino_VCC.h> 
#include <iarduino_OLED_txt.h>                         
#include <IRremote.hpp>
#include <SoftwareSerial.h>

#include "define_master_kru.h"  // подключили определения управляющей системы 
#include "common_master_kru.h"  // подключили общие функции управляющей системы 

void setup() 
{
   myOLED.begin();                                          // Инициируем работу с дисплеем.
   myOLED.setFont(MediumFontRus);                           // Указываем шрифт который требуется использовать для вывода цифр и текста.

   serialMaster.begin(300);  

   pinMode(buzPin, OUTPUT);  
   digitalWrite(buzPin, LOW);
   IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);

   pinMode(LEDPIN, OUTPUT);
 
   cli(); // stop interrupts
   // set timer1 interrupt at 1Hz
   TCCR1A = 0; // set entire TCCR1A register to 0
   TCCR1B = 0; // same for TCCR1B
   TCNT1  = 0; // initialize counter value to 0
   // set compare match register for 1hz increments
   OCR1A = 15624; // = (16*10^6) / (1*1024) - 1 (must be <65536)
   // turn on CTC mode
   TCCR1B |= (1 << WGM12);
   // Set CS12 and CS10 bits for 1024 prescaler
   TCCR1B |= (1 << CS12) | (1 << CS10);  
   // enable timer compare interrupt
   TIMSK1 |= (1 << OCIE1A);
   sei(); // allow interrupts
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
// ****************************************************************************
// *                         Отметить истечение 1 секунды                     *
// ****************************************************************************
ISR(TIMER1_COMPA_vect)
{
   // Устанавливаем флаг прошествия 1 секунды. Флаг всегда устанавливаем в true 
   // для того, чтобы основной цикл знал, что секунда истекла 
   // (основной цикл по своей логике и сбросит флаг в false)
   OneSecondFlag = true;
}

// ******************************************************** mk05-IniMen.ino ***
