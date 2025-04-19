/** Arduino, Esp32-CAM ************************************ define_kvizzy.h ***
 * 
 *                 Определить переменные и константы исполнительного приложения 
 *                                    умного хозяйства на контроллере ESP32-CAM
 * 
 * v4.0.0, 28.03.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>

// Управляем включением задач
// #define tmr_LEAD
// #define tmr_STATE   // 2025-03-08, state не отключаем, пусть сообщения обрабатываются
#define tmr_STREAM

// Вводим имя и пароль точки доступа
// const char* ssid     = "OPPO A9 2020";
// const char* password = "b277a4ee84e8";
const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

// Определяем пин вспышки
#define BUILTIN_LED 4

// Определяем сайт умного хозяйства  
//String SiteHost="/home/u542632";
String urlHome="https://probatv.ru";
// Назначаем задачи, инициируем флаги для сторожевого таймера
void vPrint(void *pvParameters);
bool fwdtPrint = false;
void vStream(void *pvParameters);
bool fwdtStream = false;
void vLed4(void *pvParameters);
bool fwdtLed4 = false;
void vOTA(void *pvParameters);
bool fwdtOTA = false;
//void vLead(void *pvParameters);
//bool fwdtLead = false;
//void vState(void *pvParameters);
//bool fwdtState = false;
//void vLed33(void *pvParameters);
//bool fwdtLed33 = false;
// Сбрасываем флаг базового цикла приложения
bool fwdtLoop = false;

/*
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
bool toTrassState=false;       // "выключена трассировка сообщений к State" 
bool fromTrassState=false;     // "выключена трассировка ответов от State" 

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
*/

// Определяем структуру информации по запросу к странице сайта
struct tQueryMessage
{
   int  httpCode;        // код ответа на запрос
   String httpText;      // текст ответа (max 32768 символов)
};

/*
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
*/

// ******************************************************** define_kvizzy.h ***
