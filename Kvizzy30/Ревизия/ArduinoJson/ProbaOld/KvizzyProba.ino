/** Arduino, Esp32-CAM *************************************** KvizzyProba.ino ***
 * 
 * Kvizzy30 - модельное программное обеспечение для Esp32-CAM и двух встроенных
 * светодиодов: контрольного и вспышки
 * 
 * v3.1, 07.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 31.05.2024
 * 
 *           Kvizzy - система контроллеров, датчиков и исполнительных устройств 
 *                    моего, стремящегося к умному, хозяйства (нижний уровень).
 * 
**/


// Размещаем JSON-документ
#include <ArduinoJson.h>
JsonDocument doc;

#include "define_kvizzy.h"   // подключили общие определения 
#include "common_kvizzy.h"   // подключили общие функции  

void setup() 
{
   Serial.begin(115200);
   while (!Serial) continue;
   
   String sjson=jison1();
   Serial.println(sjson);
   ssetup(sjson);

   sjson=thisController();
   Serial.print("Контроллер0: ");
   Serial.println(sjson);


   /*
   // The filter: it contains "true" for each value we want to keep
   JsonDocument filter;
   //filter["list"][0]["dt"] = true;
   //filter["list"][0]["main"]["temp"] = true;

   filter["namectrl"] = true; // "Esp32-CAM во двор дачи";
   filter["idplace"]  = true; // 'Во двор дачи'
   filter["tidctrl"]  = true; // 'Esp32-CAM'
   filter["idctrl"]   = true; // идентификатор контроллера
   
   // Deserialize the document
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   // Print the result
   String str = "";
   serializeJson(doc,str);
   Serial.print("Контроллер1: ");
   Serial.println(str);
   */
   String str=thisA(sjson);
   Serial.print("КонтроллерA: ");
   Serial.println(str);

   str=thisB(sjson);
   Serial.print("КонтроллерB: ");
   Serial.println(str);


   /*
   filter["idctrl"]   = false; // идентификатор контроллера
   filter["sensor"]   = true;  
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   // Print the result
   str = "";
   serializeJson(doc,str);
   Serial.print("Контроллер2: ");
   Serial.println(str);
   */




   
}

void loop() 
{
}

void ssetup(String g) 
{
  // JSON input string.
  // const char* json = "12{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  const char* json = g.c_str();


  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  const char* sensor = doc["sensor"];
  long time = doc["time"];
  double latitude = doc["data"][0];
  double longitude = doc["data"][1];

  // Print values.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);
}


// *********************************************************** KvizzyProba.ino ***
