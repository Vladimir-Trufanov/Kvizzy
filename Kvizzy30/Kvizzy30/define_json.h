/** Arduino, Esp32-CAM ************************************** define_json.h ***
 * 
 *                                          Обеспечить работу с документом JSON
 * 
 * v1.3, 25.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#ifndef defineJson
#define defineJson
#pragma once            

#include <Arduino.h>

// Размещаем JSON-документ
#include <ArduinoJson.h>
JsonDocument doc;

String jempty = "{}";  // пустая json-строка
String sjson;          // выборка json-ljrevtynf

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
   led_33["light"]  = 55;           // процент времени свечения в цикле     
   led_33["time"]   = 4;            // длительность цикла "горит - не горит" (мсек)     
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
   DeserializationError error = deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   // Test if parsing succeeds.
   if (error) 
   {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      // return;
   }
   String str = "";
   serializeJson(doc,str);
   return str;
}

#endif

// ********************************************************** define_json.h ***
