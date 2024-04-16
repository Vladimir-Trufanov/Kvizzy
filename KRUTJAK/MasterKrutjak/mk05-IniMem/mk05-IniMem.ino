/** KRUTJAK-MASTER-PRO_MINI                             *** mk05-IniMen.ino ***
 * 
 * mk05 - Вывести в отдельный модуль переменные и константы 
 *        управляющей системы паровозика "КРУТЯК"
 * 
 * v3.0, 16.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

#include <iarduino_VCC.h> 
#include <iarduino_OLED_txt.h>                         
#include <IRremote.hpp>
#include <SoftwareSerial.h>

#include "define_master_kru.h"  // подключили определения управляющей системы 

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
   
   if (IrReceiver.decode()) 
   {
      // myOLED.clrScr(); // почистили экран

      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      command = IrReceiver.decodedIRData.command;
      // Отправляем команду в Slave по VirtUART
      sCommand = String(command);
      if (command<10) sCommand="0"+sCommand;
      serialMaster.println("AT+"+sCommand+".");
      // Делаем задержку в 100 мс, чтобы отработать сигнал от нажатия клавиши 
      delay(100);  
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
      viewState(sCommand);
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

/*
// ****************************************************************************
// *                Отправить команду исполнительной системе                  *
// ****************************************************************************
String putCom(uint16_t command)
{
   String sCommand = String(command);
   // Формируем код команды
   if (command<10)
   {
      sCommand="0"+sCommand;
   }
   sCommand="AT+"+sCommand+".";
   return sCommand; 
}
*/

// ****************************************************************************
// *                Отправить команду исполнительной системе                  *
// ****************************************************************************
void viewState(String sCommand)
{
   cli();
   // Выводим последнюю команду, начиная с 80 столбца 1 строки 
   // (высота шрифта 2 строки, текст займёт строки 0 и 1).
   if (sCommand != oldCommand)
   {
      myOLED.print(sCommand,80,1); 
      oldCommand=sCommand;           
   }
   
   // myOLED.print(F("ком"),   0,     1);  
   // myOLED.print(command,  40,     1);  
   
   // Выводим текст начиная с 15 столбца 4 строки (высота шрифта 2 строки, он займёт строки 3 и 4).
   myOLED.print(F("iArduino"),15,4); 
   
   // Выводим напряжения батарей
   // myOLED.print(F("v"),0,7); myOLED.print(F(":"),10,7);                 
   // myOLED.print(F("4.99"),22,7); myOLED.print(F("-"),69,7); myOLED.print(analogRead_VCC(),80,7);                   
   sei();
}

// ******************************************************** mk05-IniMen.ino ***
