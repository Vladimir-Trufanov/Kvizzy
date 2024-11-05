// ServerPing.ino

#include <WiFi.h>
#include <ESP32Ping.h>

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  //bool success = Ping.ping("https://kwinflat.ru/Dacha/",10);
  bool success = Ping.ping("www.kwinflat.ru",10);
  if (!success) 
  {
    Serial.println("Ping failed");
    return;
  }
  Serial.println("Ping succesful.");
  
  // Определяем среднее время отклика (в миллисекундах) 
  float avg_time_ms = Ping.averageTime();
  Serial.print("Среднее время отклика: ");
  Serial.println(avg_time_ms);
}

void loop() 
{ 
}
