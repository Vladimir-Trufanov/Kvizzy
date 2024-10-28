/** Arduino, Esp32-CAM ************************************ define_kvizzy.h ***
 * 
 * Определить переменные и константы нижнего уровня умного хозяйства на двух светодиодах
 * 
 * v1.1, 26.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#ifndef define_kvizzy
#define define_kvizzy
#pragma once            

#include <Arduino.h>

// Определяем состояния светодиода с обратной логикой
#define inHIGH LOW
#define inLOW  HIGH 

// Определяем общий JSON-документ контроллера и его датчиков и оборудования
// https://arduinojson.org/ 
String thisController() 
{
   // Включаем в документ данные контроллера
   doc["namectrl"] = "Esp32-CAM во двор дачи";
   doc["idctrl"] = 1;               // идентификатор контроллера
   doc["tidctrl"]  = 1;             // идентификатор типа контроллера
   doc["typectrl"] = "Esp32-CAM";   // тип контроллера

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
   led_33["iddev"] = 2;             // идентификатор устройства
   led_33["tiddev"] = 2;            // идентификатор типа устройства
   led_33["typedev"] = "inLed";     // тип устройства
   led_33["status"] = inLOW;        // текущее состояние светодиода     

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

String getCore0(String sjson) 
{
   JsonDocument filter;
   filter["idctrl"] = true; 
   filter["core0"][0]["iddev"]= true;
   filter["core0"][0]["minstack"]= true;
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   doc["core0"][0]["minstack"]=34;
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

/*
const int PinRes   = A0;  // аналоговый вход, к которому подключен потенциометр
const int PinPWM_L = 10;  // цифровой (ШИМ) выход, к которому подключено левое плечо транзисторов
const int PinPWM_R = 9;   // цифровой (ШИМ) выход, к которому подключено правое плечо

#define LEDPIN         13 
#define IR_RECEIVE_PIN 2


// Состояние светодиода: "горит" - "не горит"
bool doBurns;
// Определяем счетчик прерываний от таймера и общее их количество до
// события переключения светодиода = 1 секунде 
volatile unsigned int cntr;
const unsigned int BtnToggle = 62499;
// Определяем флаг истечения 1 сек для запуска трассировок
volatile boolean OneSecondFlag = false;
*/

#endif

// ******************************************************** define_kvizzy.h ***
