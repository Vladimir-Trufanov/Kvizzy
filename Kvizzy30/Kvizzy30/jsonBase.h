/** Arduino, Esp32-CAM ***************************************** jsonBase.h ***
 * 
 *                                          Обеспечить работу с документом JSON
 * 
 * v1.3.6, 29.01.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>
#include <ArduinoJson.h>

// Реестр json-сообщений на страницу State
const String s33_HIGH  = "{\"led33\":[{\"status\":\"inHIGH\"}]}";  // "контрольный светодиод включен"
const String s33_LOW   = "{\"led33\":[{\"status\":\"inLOW\"}]}";   // "контрольный светодиод ВЫКЛЮЧЕН"
const String s33_MODE0 = "{\"led33\":[{\"regim\":0}]}";            // "режим контрольного светодиода выключен"

String jempty = "{}";  // пустая json-строка
String sjson;          // выборка из json-документа

/*
sla6en9edged
browscap=/home/kwinflatht/browscap.ini
browscap=/home/u542632/browscap.ini
*/

class TJsonBase
{
   public:

   // Определить объект (конструктор класса)
   TJsonBase();
   // Создать объект и строку всего JSON-документа
   void Create();
   // Выбрать из JSON-документа режим и состояние контрольного светодиода
   String jsongetLed33();
   // ---Определить объект (конструктор класса)        
   void jsonset();
   // Показать текущее состояние json-документа            
   void ViewDoc();
   // Обновить json-документ
   void UpdateDoc(String sjson);
   
   private:

   JsonDocument doc;      // JSON-документ
   String AllJson;        // строка всего документа
};
// ****************************************************************************
// *                  Определить объект (конструктор класса)                  *
// ****************************************************************************
TJsonBase::TJsonBase()
{
}
// ****************************************************************************
// *               Создать объект и строку всего JSON-документа               *
// ****************************************************************************
void TJsonBase::Create()
{
   /*
   // Идентификаторы и типы контроллеров "стремящегося к умному хозяйства" (ControllersType)
   tidctrl typectrl
      1,'Esp32-CAM'
      2,'Arduino Pro Mini'
      3,'Esp01'
   // Идентификаторы и типы устройств (DevicesType)
   tiddev typedev
      1,'inLed'      // светодиод c обратной логикой
      2,'Led'        // светодиод
      3,'Core32'     // ядро Esp32
   // Идентификаторы и типы датчиков (SensorsType)
   tidsens typesens
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
   core_0["nicdev"] = "core0";      // nic устройства
   core_0["tiddev"] = 3;            // идентификатор типа устройства
   core_0["idcore"] = 0;            // идентификатор ядра
   core_0["stacksize"] = 0;         // выделенный размер стека
   core_0["minstack"] = 0;          // минимальный отмеченный размер
   // Первое "1" ядро контроллера
   JsonArray core1 = doc.createNestedArray("core1");
   JsonObject core_1 = core1.createNestedObject();
   core_1["nicdev"] = "core1";      // nic устройства
   core_1["tiddev"] = 3;            // идентификатор типа устройства
   core_1["idcore"] = 1;            // идентификатор ядра
   core_1["stacksize"] = 0;         // выделенный размер стека
   core_1["minstack"] = 0;          // минимальный отмеченный размер
   // Контрольный светодиод, режим работы, состояние
   JsonArray led33 = doc.createNestedArray("led33");
   JsonObject led_33 = led33.createNestedObject();
   led_33["nicdev"] = "led33";      // nic устройства
   led_33["tiddev"] = 1;            // идентификатор типа устройства
   led_33["light"]  = 10;           // процент времени свечения в цикле     
   led_33["time"]   = 2000;         // длительность цикла "горит - не горит" (мсек)     
   led_33["regim"]  = 1;            // режим работы: 0 - выключен, 1 - включён     
   led_33["status"] = "inLOW";      // текущее состояние светодиода     
   // Вспышка
   JsonArray led4 = doc.createNestedArray("led4");
   JsonObject led_4 = led4.createNestedObject();
   led_4["nicdev"]  = "led4";       // nic устройства
   led_4["tiddev"]  = 2;            // идентификатор типа устройства
   led_4["status"]  = "LOW";        // текущее состояние светодиода 
   // Выбираем весь json-документ в строку
   serializeJson(doc,AllJson);
   //serializeJsonPretty(doc,Serial); 
}
// ****************************************************************************
// *    Выбрать из JSON-документа режим и состояние контрольного светодиода   *
// ****************************************************************************
String TJsonBase::jsongetLed33()
/*
{
  "led33": [
    {
      "nicdev": "led33",
      "tiddev": 1,
      "light": 10,
      "time": 2000,
      "regim": 1,
      "status": "inLOW"
    }
  ]
}

sjson={"led33":[{"nicdev":"led33","tiddev":1,"light":10,"time":2000,"regim":1,"status":"inLOW"}]}
*/
{
   // Инициируем возвращаемую json-строку   
   String sjson = jempty;
   // Формируем json по состоянию контрольного светодиода 
   JsonDocument filter;
   filter["led33"][0]["nicdev"]= true;  // nic устройства
   filter["led33"][0]["tiddev"]= true;  // идентификатор типа устройства
   filter["led33"][0]["light"] = true;  // процент времени свечения в цикле 
   filter["led33"][0]["time"]  = true;  // длительность цикла "горит - не горит" (мсек)
   filter["led33"][0]["regim"] = true;  // режим работы: 0 - выключен, 1 - включён  
   filter["led33"][0]["status"]= true;  // текущее состояние светодиода     
   JsonDocument doc;
   // Заполняем фильтр данными из всего документа
   deserializeJson(doc,AllJson,DeserializationOption::Filter(filter));
   // Формируем json-строку с информацией по контрольному светодиоду
   serializeJson(doc,sjson);
   return sjson;
}
/*
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
*/
// ****************************************************************************
// *                  Показать текущее состояние json-документа               *
// ****************************************************************************
void TJsonBase::ViewDoc()
{
   serializeJsonPretty(doc,Serial);
}
// ****************************************************************************
// *                             Обновить json-документ                       *
// ****************************************************************************
void TJsonBase::UpdateDoc(String sjson)
{
   // Показываем предыдущее состояние json-документа
   // ViewDoc(); 

   // "контрольный светодиод включен" -            {"led33":[{"status":"inHIGH"}]}
   if (sjson==s33_HIGH)                            doc["led33"][0]["status"] = "inHIGH";

   // "контрольный светодиод ВЫКЛЮЧЕН" -           {"led33":[{"status":"inLOW"}]}
   else if (sjson==s33_LOW)                        doc["led33"][0]["status"] = "inLOW";

   // "режим контрольного светодиода выключен" -   {"led33":[{"regim":0}]}
   else if (sjson==s33_MODE0)                      doc["led33"][0]["regim"] = 0;
   else 
   { 
      // Safe! Continue usual tasks.
   }
   // Пересобираем json (строку) всего документа
   serializeJson(doc,AllJson);
   // Показываем текущее состояние json-документа
   // ViewDoc(); 
   // Serial.println(AllJson);
}

// ************************************************************* jsonBase.h ***
