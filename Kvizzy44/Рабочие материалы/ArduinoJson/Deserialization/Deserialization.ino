// Deserialization.ino

#include <ArduinoJson.h>

void setup() 
{
  // Initialize serial port
  Serial.begin(115200);
  while (!Serial)continue;

  // Allocate the JSON document
  JsonDocument doc;

  // JSON input string.
  const char* json = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038],\"slovo\":\"Русское\"}";

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
  double longitude = doc["data"][1];
  const char* sensor = doc["sensor"];
  long time = doc["time"];
  double latitude = doc["data"][0];
  const char* slovo = doc["slovo"];

  // Print values.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude,3);
  Serial.println(slovo);
}

void loop() 
{
  // not used in this example
}
