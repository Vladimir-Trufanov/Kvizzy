/** Arduino, Esp32-CAM *************************************** LearningJSON.ino ***
 * 
 * Ознакомиться с JSON (с сериализацией и десериализацией)
 * 
 * v3.1, 07.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 31.05.2024
**/

#include "common_kvizzy.h"   // подключили общие функции  

void setup() 
{
   Serial.begin(115200);
   while (!Serial) continue;
   String g=jison1();
   Serial.println(g);

   while (!Serial) continue;
   ssetup(g);
}

void loop() 
{
}

void ssetup(String g) 
{

  // Allocate the JSON document
  JsonDocument doc;

  // Создаем ошибочны JSON
  // const char* json = "12{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  // JSON input string.
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

// *********************************************************** LearningJSON.ino ***
