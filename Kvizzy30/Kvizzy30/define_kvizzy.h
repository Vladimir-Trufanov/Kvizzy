/** Arduino, Esp32-CAM ************************************ define_kvizzy.h ***
 * 
 * Определить переменные и константы нижнего уровня умного хозяйства на двух светодиодах
 * 
 * v1.2, 08.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#ifndef define_kvizzy
#define define_kvizzy
#pragma once            

#include <Arduino.h>

// Значения чисел, считанных из последовательного порта, 
// иммитирующие ошибочные ситуации 
#define loopingLed33     1     // зацикливание задачи vLed33
#define loopingTastes    2     // зацикливание задачи vTastes
#define loopingCore1     3     // зацикливание задачи vCore1
#define loopingCore0     4     // зацикливание задачи vCore0
#define disaflashing33  51     // отключение мигания контрольного светодиода
#define enabflashing33  52     // включение мигания контрольного светодиода

// Управление светодиодами
#define PinLedWork      33            // контакт рабочего светодиода
volatile int lastLedWork = millis();  // прошлое время смены состояния контрольного светодиода
volatile int millLedWork = 10017;     // текущий интервал смены состояния контрольного светодиода
#define PinLedFlash      4            // контакт светодиода-вспышки

// Назначаем задачу определение состояния контрольного светодиода ESP32-CAM 
// ("горит - не горит") и передачу данных на страницу сайта State  
void vLed33(void *pvParameters);
// Определяем флаг задачи для сторожевого таймера
bool fwdtLed33 = false;

// Определяем задачи и их флаги
//void vTastes(void *pvParameters);
void vCore1(void *pvParameters);
void vCore0(void *pvParameters);
int flag[] = {0, 0, 0, 0};

// Определяем число, которое будет считываться в основном цикле
// с последовательного порта
volatile int iCreateSit;

//
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

// Размещаем JSON-документ
#include <ArduinoJson.h>
JsonDocument doc;

// Определяем пустую json-строку
String jempty = "{}";

String sjson;

// Определяем общий JSON-документ контроллера и его датчиков и оборудования
// https://arduinojson.org/ 
String thisController() 
{
   /*
   // Идентификаторы и типы контроллеров "стремящегося к умному хозяйства" (ControllersType)
      tidctrl typectrl
      ----------------
            1,'Esp32-CAM'
            2,'Arduino Pro Mini'
            3,'Esp01'

   // Идентификаторы и типы устройств (DevicesType)
       tiddev typedev
       --------------
            1,'inLed'      // светодиод c обратной логикой
            2,'Led'        // светодиод
            3,'Core32'     // ядро Esp32
  
   // Идентификаторы и типы датчиков (SensorsType)
      tidsens typesens
      ----------------
            1,'DHT11'         
            2,'DHT22'       
   */

   // Включаем в документ данные контроллера
   doc["tidctrl"] = 1;                         // идентификатор типа контроллера
   doc["namectrl"] = "Esp32-CAM во двор дачи"; //
   doc["nicctrl"] = "myjoy";                   // nic контроллера
   // Нулевое "0" ядро контроллера
   JsonArray core0 = doc.createNestedArray("core0");
   JsonObject core_0 = core0.createNestedObject();
   core_0["iddev"] = 0;             // идентификатор устройства
   core_0["tiddev"] = 3;            // идентификатор типа устройства
   core_0["typedev"] = "Core32";    // тип устройства
   core_0["idcore"] = 0;            // идентификатор ядра
   core_0["stacksize"] = 0;         // выделенный размер стека
   core_0["minstack"] = 0;          // минимальный отмеченный размер
   // Первое "1" ядро контроллера
   JsonArray core1 = doc.createNestedArray("core1");
   JsonObject core_1 = core1.createNestedObject();
   core_1["iddev"] = 1;             // идентификатор устройства
   core_1["tiddev"] = 3;            // идентификатор типа устройства
   core_1["typedev"] = "Core32";    // тип устройства
   core_1["idcore"] = 1;            // идентификатор ядра
   core_1["stacksize"] = 0;         // выделенный размер стека
   core_1["minstack"] = 0;          // минимальный отмеченный размер
   // Контрольный светодиод
   JsonArray led33 = doc.createNestedArray("led33");
   JsonObject led_33 = led33.createNestedObject();
   led_33["tiddev"] = 1;            // идентификатор типа устройства
   led_33["nicdev"] = "inLed";      // nic устройства
   led_33["status"] = "inLOW";      // текущее состояние светодиода     
   // Вспышка
   JsonArray led4 = doc.createNestedArray("led4");
   JsonObject led_4 = led4.createNestedObject();
   led_4["iddev"] = 3;              // идентификатор устройства
   led_4["tiddev"] = 2;             // идентификатор типа устройства
   led_4["typedev"] = "Led";        // тип устройства
   led_4["status"] = LOW;           // текущее состояние светодиода     
   // Датчик температуры
   JsonArray DHT22 = doc.createNestedArray("DHT22");
   JsonObject DHT_22 = DHT22.createNestedObject();
   DHT_22["iddev"] = 4;             // идентификатор устройства
   DHT_22["tiddev"] = 5;            // идентификатор типа устройства
   DHT_22["typedev"] = "DHT22";     // тип устройства
   DHT_22["status"] = 0;            // текущее состояние светодиода     

   String str = "";
   serializeJson(doc,str);
   serializeJsonPretty(doc,Serial);
   return str;
}

String getEsp32CAM(String sjson) 
{
   JsonDocument filter;
   filter["namectrl"] = true;  // "Esp32-CAM во двор дачи";
   filter["idctrl"] = true; 
   filter["tidctrl"] = true; 
   filter["typectrl"]  = true; // 'Esp32-CAM'
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   String str = "";
   serializeJson(doc,str);
   return str;
}

String getCoreX(String sjson, String imjaCore="core1") 
{
   JsonDocument filter;
   filter["idctrl"] = true; 
   filter[imjaCore][0]["iddev"]= true;
   filter[imjaCore][0]["minstack"]= true;
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   doc[imjaCore][0]["minstack"]=34;
   String str = "";
   serializeJson(doc,str);
   return str;
}

String getDHT22(String sjson) 
{
   JsonDocument filter;
   filter["idctrl"] = true; 
   filter["DHT22"][0]["iddev"]= true;
   filter["DHT22"][0]["status"]= true;
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   doc["DHT22"][0]["status"]=128;
   String str = "";
   serializeJson(doc,str);
   return str;
}

#endif

// ******************************************************** define_kvizzy.h ***
