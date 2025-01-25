/** Arduino, Esp32-CAM *************************************** KvizzyProba.ino ***
 * 
 *                         Kvizzy - пример создания Json-документа и его изменения
 * 
 * v3.1, 25.01.2025                                      Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                                  Дата создания: 31.05.2024
**/
// Размещаем JSON-документ
#include <ArduinoJson.h>
JsonDocument doc;

#include "define_kvizzy.h"   // подключили общие определения 

void setup() 
{
   Serial.begin(115200);
   while (!Serial) continue;
   // Строим json-документ
   String sjson=thisController();
   Serial.println("Контроллер:");
   Serial.println(sjson);
   Serial.println("");
   // Строим выжимку json-документа
   String str=thisMake(sjson);
   Serial.println("Изменение:");
   Serial.println(str);
   Serial.println("");
   // Делаем замену в глобальном json-документе
   str=Proba(sjson);
   Serial.println("Proba:");
   Serial.println(str);
   Serial.println("");
   // Показываем вариант обработки ошибок
   //String g = "12{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
   String g = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
   ssetup(g); 
}

void loop() 
{
}

// Показать вариант обработки ошибок
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
