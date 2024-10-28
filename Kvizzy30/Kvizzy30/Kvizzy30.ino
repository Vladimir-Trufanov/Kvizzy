/** Arduino, Esp32-CAM *************************************** Kvizzy30.ino ***
 * 
 * Kvizzy30 - модельное программное обеспечение для Esp32-CAM и двух встроенных
 * светодиодов: контрольного и вспышки
 * 
 * v3.1, 28.10.2024                                   Автор:      Труфанов В.Е.
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
   
   String sjson=thisController();

   Serial.println("");
   String str=getEsp32CAM(sjson);
   Serial.print("Контроллер: ");
   Serial.println(str);

   str=getCore0(sjson);
   Serial.print("getCore0: ");
   Serial.println(str);

   str=getDHT22(sjson);
   Serial.print("getDHT22: ");
   Serial.println(str);

   ssetup(sjson); 
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
  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Fetch values.
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  const char* namectrl = doc["namectrl"];
  double minstack = doc["core0"][0]["minstack"];
  int DHT22status = doc["DHT22"][0]["status"];
  // Print values.
  Serial.println(namectrl);
  Serial.println(minstack,2);
  Serial.println(DHT22status);
}

// *********************************************************** Kvizzy30.ino ***
