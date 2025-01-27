/** Arduino, Esp32-CAM ************************************ define_kvizzy.h ***
 * 
 * Определить переменные и константы нижнего уровня умного хозяйства на двух светодиодах
 * 
 * v3.3.4, 27.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>

// Определяем 
String shttp="http://probatv.ru/";     // сайт верхнего уровня
String inMess="";                      // метка отправленного сообщения

// Значения чисел, считанных из последовательного порта, 
// иммитирующие ошибочные ситуации 
#define loopingLed33     1     // зацикливание задачи vLed33
#define loopingState     2     // зацикливание задачи vState
#define loopingCore1     3     // зацикливание задачи vCore1
#define loopingCore0     4     // зацикливание задачи vCore0
//#define disaflashing33  51     // отключение мигания контрольного светодиода
//#define enabflashing33  52     // включение мигания контрольного светодиода

// Управление трассировкой сообщений к State 
#define enabTrassState   53    // включение трассировки сообщений к State
#define disaTrassState   54    // отключение 
bool isTrassState=true;        // "выключена трассировка сообщений к State" 

// Инициируем счетчики циклов задач (от 0 до 4 294 967 295)
// и флаги включения режимов работы 
uint32_t iLead=0;       // запрос к странице Lead
uint32_t iState=0;      // запрос к странице State
bool Led33Start=true;   // включить режим работы контрольного светодиода

// Управление светодиодами
#define PinLedWork      33            // контакт рабочего светодиода
volatile int lastLedWork = millis();  // прошлое время смены состояния контрольного светодиода
volatile int millLedWork = 3017;      // текущий интервал смены состояния контрольного светодиода (чуть более 3 секунд)
#define PinLedFlash      4            // контакт светодиода-вспышки

// Назначаем задачи и флаги сторожевого таймера
void vLead(void *pvParameters);
bool fwdtLead = false;

void vState(void *pvParameters);
bool fwdtState = false;

void vLed33(void *pvParameters);
bool fwdtLed33 = false;

void vPrint(void *pvParameters);
bool fwdtPrint = false;

//void vCore0(void *pvParameters);
//bool fwdtCore0 = false;
//void vCore1(void *pvParameters);
//bool fwdtCore1 = false;

// Сбрасываем флаг основного цикла
bool fwdtLoop = false;

// Определяем структуру информации по запросу к странице сайта
struct tQueryMessage
{
   int  httpCode;        // код ответа на запрос
   String httpText;      // текст ответа (max 32768 символов)
};

// Определяем число, которое будет считываться в основном цикле
// с последовательного порта - команду инициирования зависания процессора
volatile int iCreateSit;
// Определяем временную метку сообщения о зависании процессора
volatile int mitMimic=millis();
// Имитируем событие зависания процессора
void MimicMCUhangEvent(String NameTask)
{
   while (true)
   {
      int mitMimici=millis();
      if ((mitMimici-mitMimic)>1005)
      {
         Serial.print(NameTask);
         Serial.println(": зависание процессора!!!");
         mitMimic = mitMimici;
      }
   }
}

// ******************************************************** define_kvizzy.h ***
