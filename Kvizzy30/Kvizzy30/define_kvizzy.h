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
String thisController() 
{
   // Включаем в документ данные контроллера
   
   doc["namectrl"] = "Esp32-CAM во двор дачи";
   doc["tidctrl"]  = 1;            // идентификатор типа контроллера
   doc["typectrl"] = "Esp32-CAM";  // тип контроллера
   
   // Включаем в документ встроенные или подключенные устройства
   JsonArray devices = doc.createNestedArray("devices");

   // Нулевое "0" ядро контроллера
   JsonObject device0 = devices.createNestedObject();
   device0["iddev"] = 0;           // идентификатор устройства
   device0["tiddev"] = 3;          // идентификатор типа устройства
   device0["typedev"] = "Core32";  // тип устройства
   JsonArray core_0 = device0.createNestedArray("core_0");
   JsonObject core0 = core_0.createNestedObject();
   core0["idcore"] = 0;            // идентификатор ядра
   core0["stacksize"] = 0;         // выделенный размер стека
   core0["minstack"] = 0;          // минимальный отмеченный размер

   // Первое "1" ядро контроллера
   JsonObject device1 = devices.createNestedObject();
   device1["iddev"] = 1;           // идентификатор устройства
   device1["tiddev"] = 3;          // идентификатор типа устройства
   device1["typedev"] = "Core32";  // тип устройства
   JsonArray core_1 = device1.createNestedArray("core_1");
   JsonObject core1 = core_1.createNestedObject();
   core1["idcore"] = 1;            // идентификатор ядра
   core1["stacksize"] = 0;         // выделенный размер стека
   core1["minstack"] = 0;          // минимальный отмеченный размер

   // Контрольный светодиод
   JsonObject device2 = devices.createNestedObject();
   device2["iddev"] = 2;           // идентификатор устройства
   device2["tiddev"] = 1;          // идентификатор типа устройства
   device2["typedev"] = "inLed";   // тип устройства
   JsonArray led_33 = device2.createNestedArray("led_33");
   JsonObject led33 = led_33.createNestedObject();
   led33["status"] = inLOW;        // текущее состояние светодиода     

   // Вспышка
   JsonObject device3 = devices.createNestedObject();
   device3["iddev"] = 3;           // идентификатор устройства
   device3["tiddev"] = 2;          // идентификатор типа устройства
   device3["typedev"] = "Led";     // тип устройства
   JsonArray led_4 = device3.createNestedArray("led_4");
   JsonObject led4 = led_4.createNestedObject();
   led4["status"] = LOW;           // текущее состояние светодиода     
   
   // Датчик температуры
   JsonObject device4 = devices.createNestedObject();
   device4["iddev"] = 4;           // идентификатор устройства
   device4["tiddev"] = 5;          // идентификатор типа устройства
   device4["typedev"] = "DHT22";   // тип устройства
   JsonArray DHT_22 = device4.createNestedArray("DHT_22");
   JsonObject DHT22 = DHT_22.createNestedObject();
   DHT22["status"] = 0;            // текущее показание датчика     
   
   String str = "";
   //serializeJson(doc,str);
   serializeJsonPretty(doc,str);

   return str;
}

String thisA(String sjson) 
{
   JsonDocument filter;
   filter["namectrl"] = true; // "Esp32-CAM во двор дачи";
   filter["idplace"]  = true; // 'Во двор дачи'
   filter["tidctrl"]  = true; // 'Esp32-CAM'
   filter["idctrl"]   = true; // идентификатор контроллера
   // Deserialize the document
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   String str = "";
   serializeJson(doc,str);
   return str;
}

String thisB(String sjson) 
{
   JsonDocument filter;
   filter["sensor"] = true; 
   filter["time"]  = true; 
   filter["data"]  = true; 
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   doc["data"][0]=1.0;
   doc["data"][1]=1.1;
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
