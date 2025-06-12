/** Arduino, Esp32-CAM ************************************ define_kvizzy.h ***
 * 
 *                 Определить переменные и константы исполнительного приложения 
 *                                    умного хозяйства на контроллере ESP32-CAM
 * 
 * v4.5.0, 12.06.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve      sla6en9edged             Дата создания: 26.10.2024
**/

// Простые числа: https://calculat.io/ru/number/prime/1--3000
// Preferences:   https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html

#pragma once            
#include <Arduino.h>
//#include <Preferences.h>

// Управляем включением задач
#define tmr_LEAD
#define tmr_STATE   
#define tmr_STREAM

// Вводим имя и пароль точки доступа
// const char* ssid     = "OPPO A9 2020";
// const char* password = "b277a4ee84e8";
const char* ssid     = "TP-Link_B394";
const char* password = "18009217";
// Определяем пин вспышки
#define LED_PIN_4  4        

// Определяем сайт умного хозяйства  
//String SiteHost="/home/u542632";
String urlHome   = "https://probatv.ru";
String urlLead   = "Lead40";
String urlStream = "Stream40";
String urlState  = "State40";
// Назначаем задачи, инициируем флаги для сторожевого таймера
void vPrint(void *pvParameters);
bool fwdtPrint = false;
void vStream(void *pvParameters);
bool fwdtStream = false;
void vLed4(void *pvParameters);
bool fwdtLed4 = false;
void vDHT11(void *pvParameters);
bool fwdtDHT11 = false;
void vLead(void *pvParameters);
bool fwdtLead = false;
//void vOTA(void *pvParameters);
//bool fwdtOTA = false;
// Сбрасываем флаг базового цикла приложения
bool fwdtLoop = false;

/*
// Значения чисел, считанных из последовательного порта, 
// иммитирующие ошибочные ситуации 
#define loopingLed4      1     // зацикливание задачи vLed4
#define loopingState     2     // зацикливание задачи vState
#define loopingCore1     3     // зацикливание задачи vCore1
#define loopingCore0     4     // зацикливание задачи vCore0
//#define disaflashing33  51     // отключение мигания контрольного светодиода
//#define enabflashing33  52     // включение мигания контрольного светодиода

// Управление трассировкой сообщений к State 
#define enabTrassState   53    // включение трассировки сообщений к State
#define disaTrassState   54    // отключение 
*/
bool toTrassState=false;       // "выключена трассировка сообщений к State" 
bool fromTrassState=false;     // "выключена трассировка ответов от State" 

// Инициируем счетчики циклов задач (от 0 до 4 294 967 295)
// и флаги включения режимов работы 
uint32_t iLead=0;   // запрос к странице Lead
uint32_t iState=0;  // запрос к странице State

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

// Назначаем параметры действующего режима работы вспышки
int jlight=10;           // процент времени свечения в цикле 
int jtime=2000;          // длительность цикла "горит - не горит" (мсек)      
int inlight=10;          // принятый процент времени свечения в цикле 
int intime=2000;         // принятая длительность цикла "горит - не горит" (мсек)      
int nLight=5000000;      // время (5 сек) свечения в цикле, далее пересчитывается 
int nNoLight=5000000;    // время (5 сек) НЕ свечения в цикле, далее пересчитывается  

// Назначаем параметры показаний датчиков
int jhumi=46;            // процент влажности - "Влажность = 46%"
int jtempC=248;          // температура в десятых градусах цельсия - "Температура = 24.8°C"       

// Назначаем интервалы подачи сообщений от контроллера 
bool intrvStart=false;   // true - изменены интервалы отправки сообщений
int jmode4=  7007;       // режим работы Led4 
int jimg=    1001;       // подача изображения 
int jtempvl= 3003;       // температура и влажность 
int jlumin=  2002;       // освещённость камеры
int jbar=    5005;       // атмосферное давление

int mode4=  7007;        // принятый режим работы Led4 
int img=    1001;        // принятая подача изображения 
int tempvl= 3003;        // принятые температура и влажность 
int lumin=  2002;        // принятая освещённость камеры
int bar=    5005;        // принятое атмосферное давление

// Запрос изменений - {\"common\":0} 
const String s_COMMON = "{\"common\":0}"; 
// Режим работы вспышки - "{\"led4\":{\"light\":10,\"time\":2000}}"                                    
String s_MODE4 = "{\"led4\":{\"light\":"+String(jlight)+",\"time\":"+String(jtime)+"}}"; 
// Интервалы подачи сообщений от контроллера - "{\"intrv\":{\"mode4\":7007,\"img\":1001,\"tempvl\":3003,\"lumin\":2002,\"bar\":5005}}"
String s_INTRV = "{\"intrv\":{\"mode4\":"+String(jmode4)+",\"img\":"+String(jimg)+",\"tempvl\":"+String(jtempvl)+",\"lumin\":"+String(jlumin)+",\"bar\":"+String(jbar)+"}}"; 
// Показания температуры и влажности - "{\"dht11\":{\"humi\":46,\"tempC\":248}}"     
String s_DHT11 = "{\"dht11\":{\"humi\":"+String(jhumi)+",\"tempC\":"+String(jtempC)+"}}";  

String jempty = "{}"; // пустая json-строка
String sjson;         // выборка из json-документа

// ******************************************************** define_kvizzy.h ***
