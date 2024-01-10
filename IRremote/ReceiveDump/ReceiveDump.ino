/*
 * ReceiveDump.cpp
 *
 * Выводит полученный сигнал в различных вариантах. Поскольку печать занимает 
 * очень много времени, повторные сигналы могут быть пропущены или 
 * интерпретированы как НЕИЗВЕСТНЫЕ (UNKNOWN).
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 *  Подредактировано здесь 2023-12-30 Труфанов В.Е.
 *******************************************************************************
 * MIT License
 *
 * Copyright (c) 2020-2021 Armin Joachimsmeyer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * Настоящим бесплатно предоставляется разрешение любому лицу, получающему копию
 * данного программного обеспечения и связанных с ним файлов документации 
 * ("Программное обеспечение"), на использование Программного обеспечения без 
 * ограничений, включая, без ограничения, права использовать, копировать, 
 * изменять, объединять, публиковать, распространять, сублицензировать и/или 
 * продавать копии Программного обеспечения и разрешать лицам, которым 
 * предоставляется Программное обеспечение
 * делать это при соблюдении следующих условий:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * Вышеуказанное уведомление об авторских правах и данное уведомление о 
 * разрешении должны быть включены во все копии или существенные части 
 * Программного обеспечения.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ "КАК ЕСТЬ", БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ, 
 * ЯВНЫХ ИЛИ ПОДРАЗУМЕВАЕМЫХ, ВКЛЮЧАЯ, НО НЕ ОГРАНИЧИВАЯСЬ ГАРАНТИЯМИ ТОВАРНОЙ 
 * ПРИГОДНОСТИ, ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ И НЕНАРУШЕНИЯ ПРАВ. НИ В КОЕМ 
 * СЛУЧАЕ АВТОРЫ ИЛИ ПРАВООБЛАДАТЕЛИ АВТОРСКИХ ПРАВ НЕ НЕСУТ ОТВЕТСТВЕННОСТИ ЗА 
 * КАКИЕ-ЛИБО ПРЕТЕНЗИИ, УЩЕРБ ИЛИ ИНУЮ ОТВЕТСТВЕННОСТЬ, БУДЬ ТО В РЕЗУЛЬТАТЕ 
 * ДЕЙСТВИЯ КОНТРАКТА, ПРАВОНАРУШЕНИЯ ИЛИ ИНЫМ ОБРАЗОМ, ВЫТЕКАЮЩИХ ИЗ 
 * ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ ИЛИ В СВЯЗИ С НИМ, ИЛИ ИСПОЛЬЗОВАНИЯ ПРОГРАММНОГО 
 * ОБЕСПЕЧЕНИЯ ИЛИ ДРУГИХ СДЕЛОК С НИМ
 *
 *******************************************************************************
 */

// Пример места размещения arduino.h
// C:/Users/Евгеньевич/AppData/Local/Arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino/Arduino.h
#include <Arduino.h>

// По умолчанию RAW_BUFFER_LENGTH=112, 
// следует изменить для пультов дистанционного управления кондиционером
// #define RAW_BUFFER_LENGTH  750  

// Define macros for input and output pin etc.
#include "PinDefinitionsAndMore.h"

// Рекомендуется для недорогих модулей VS1838.
// Можно изменить это значение в зависимости от используемого модуля приемника.
// Требуемое значение может быть получено из приведенных здесь значений времени.
// Имейте в виду, что значения времени могут меняться в зависимости от расстояния
// между отправителем и получателем, а также от интенсивности окружающего освещения.
#define MARK_EXCESS_MICROS 20  
// Активация определения для некоторых протоколов кондиционирования воздуха LG
// #define RECORD_GAP_MICROS 12000 
// Активация определения для получения хороших отладочных результатов от декодера
// #define DEBUG
// Активация определения для просмотра ценной информации от универсального декодера 
// для протоколов ширины импульса или расстояния между импульсами 
#define INFO 

#include <IRremote.hpp>

void setup() 
{
   Serial.begin(115200);   // Status message will be sent to PC at 115200 baud

   delay(2000); 
   String MYCORE=setMYCORE();

   String cs="MYCORE              = "+MYCORE;

   Serial.print(F("MYCORE              = ")); Serial.println(MYCORE);
   Serial.print(F("IR_RECEIVE_PIN      = ")); Serial.println(IR_RECEIVE_PIN);
   Serial.print(F("LED_BUILTIN         = ")); Serial.println(LED_BUILTIN);
   Serial.print(F("ENABLE_LED_FEEDBACK = ")); Serial.println(ENABLE_LED_FEEDBACK);
   Serial.print(F("RAW_BUFFER_LENGTH   = ")); Serial.println(RAW_BUFFER_LENGTH);
   Serial.print(F("MARK_EXCESS_MICROS  = ")); Serial.println(MARK_EXCESS_MICROS);

   pinMode(LED_BUILTIN, OUTPUT);
   // Обеспечиваем возможность подключаться последовательному монитору после 
   // сброса или включения питания и перед первой распечаткой. 
   #if defined(__AVR_ATmega32U4__)||defined(SERIAL_USB)||defined(SERIAL_PORT_USBVIRTUAL)||defined(ARDUINO_attiny3217)
      delay(4000);
   #endif
   // Определяем версию запущенного Arduino
   Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

   // Инициируем приёмник (объект IrReceiver). Параметрами, передаваемыми в метод 
   // begin, являются номер порта (пин) на плате Arduino, к которому подключён 
   // выход датчика и константа ENABLE_LED_FEEDBACK, которая включает ретрансляции 
   // кода, полученного от датчика на светодиод. Используйте константy DISABLE_LED_FEEDBACK 
   // для отмены ретрансляции сигнала. 
   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); 

   // Включаем светодиод обратной связи, выбирая контакт обратной связи светодиода 
   // из определения внутренних плат.
   Serial.print(F("Датчик готов к приему ИК-сигналов протоколов:\r\n"));
   printActiveIRProtocols(&Serial);
   Serial.print(F("\r\nна контакте "));
   Serial.println(IR_RECEIVE_PIN);
}

void loop() 
{
   // Если пришел IR-код
   if (IrReceiver.decode()) 
   { 
      // Если буфер переполнен, то выводим сообщение
      // see also https://github.com/Arduino-IRremote/Arduino-IRremote#modifying-compile-options-with-sloeber-ide
      if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) 
      {
         Serial.println(F("Обнаружено переполнение буфера"));
         Serial.println(F("Попробуйте значение \"RAW_BUFFER_LENGTH\" сделать больше чем " STR(RAW_BUFFER_LENGTH) " в " __FILE__));
      } 
      else 
      {
         Serial.println();                               

         // Выводим информацию по коду нажатой кнопки
         Serial.println();
         IrReceiver.printIRResultShort(&Serial);
         
         // Выводим результаты в RAW формате
         /*
         Serial.println();
         Serial.println(F("Таблица результата во внутренних тиках (50 us) с управляющим интервалом"));
         IrReceiver.printIRResultRawFormatted(&Serial, false); 
         Serial.println(F("Таблица результата в микросекундах с управляющим интервалом"));
         IrReceiver.printIRResultRawFormatted(&Serial, true);  
         Serial.println(); 
         */
         
         // Выводим результат нажатой кнопки:
         /*
         // а) как массив чисел типа uint8_t, представляющих коды тиков                           
         Serial.print(F("Результат в виде массива внутренних тиков (50 us) с компенсацией MARK_EXCESS_MICROS="));
         Serial.println(MARK_EXCESS_MICROS);
         IrReceiver.compensateAndPrintIRResultAsCArray(&Serial, false); 
         // б) как массив чисел типа uint16_t, представляющих микросекунды
         Serial.print(F("Результат в виде массива микросекунд с компенсацией MARK_EXCESS_MICROS="));
         Serial.println(MARK_EXCESS_MICROS);
         IrReceiver.compensateAndPrintIRResultAsCArray(&Serial, true); 
         
         // Выводим отдельно адрес, команду и исходный код клавиши  
         IrReceiver.printIRResultAsCVariables(&Serial);  
         
         // Выводим последовательность Pronto HEX двумя способами:
         
         // а) с помощью compensateAndPrintIRResultAsPronto()
         IrReceiver.compensateAndPrintIRResultAsPronto(&Serial);
         */
         
         // б) с использованием функции compensateAndStorePronto()
         // (эти значения предназначены для Arduino UNO)
         String ProntoHEX = F("Pronto HEX contains: \r\n");        // Assign string to ProtoHex string object
         ProntoHEX = F("");
         if (int size = IrReceiver.compensateAndStorePronto(&ProntoHEX)) 
         {
            /*
            ProntoHEX += F("\r\nProntoHEX is ");              // Add codes size information to the String object
            ProntoHEX += size;
            ProntoHEX += F(" characters long and contains "); // Add codes count information to the String object
            ProntoHEX += size / 5;
            ProntoHEX += F(" codes");
            */
            Serial.println(ProntoHEX.c_str());                // Print to the serial console the whole String object
            Serial.println(); // blank line between entries
            //String ccc="0000 006D 0022 0000 0191 00AC 0018 0018 0018 0016 001A 0014 0018 0018 0018 0016 0018 0016 0018 0018 0018 0016 0018 0043 0018 0041 0018 0043 001A 0041 0018 0041 001A 0041 001A 0041 0018 0043 0018 0016 0018 0016 001A 0016 0018 0041 001A 0041 0018 0016 001A 0041 0018 0016 001A 0041 0018 0043 0018 0041 001A 0016 0018 0016 0018 0041 0018 0018 0018 0041 0018 06C3";
            //String sPronto = ProntoHEX.c_str();       
            //Serial.println(sPronto); Serial.println(ccc);  
            //if (sPronto==ccc) Serial.println("Получилось!");                              
          }
         
      }
      // Готовим прием следующего значения
      IrReceiver.resume();                          
   }
}

// ****************************************************************************
// * TCppPrown                                               void setMYCORE() *  
// *                                                                          *
// * v1.0                                            Определить текущую плату *  
// ****************************************************************************
String setMYCORE() 
{
   String MYCORE="NoDefine";
   #if defined(__AVR_ATtiny24__)||defined(__AVR_ATtiny44__)||defined(__AVR_ATtiny84__)
      MYCORE="__AVR_ATtiny24__";
   #elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
      MYCORE="__AVR_ATtiny25__";
   #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) \
   || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) \
   || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) \
   || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
      MYCORE="__AVR_ATmega1280__";
   #elif defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_STM32F1)
      MYCORE="ARDUINO_ARCH_STM32";
   #elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) \
   || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) \
   || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324A__) \
   || defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega164A__) \
   || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega32__) \
   || defined(__AVR_ATmega16__) || defined(__AVR_ATmega8535__) \
   || defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) \
   || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) \
   || defined(__AVR_ATmega8515__) || defined(__AVR_ATmega162__)
      MYCORE="__AVR_ATmega1284__";
   #elif defined(__AVR__) // Default as for ATmega328 like on Uno, Nano etc.
      MYCORE="__AVR__";
   #endif
   return MYCORE;
}
