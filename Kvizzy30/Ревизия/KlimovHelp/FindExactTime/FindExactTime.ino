#include <NTPClient.h>
#include <WiFi.h>

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

const long utcOffsetInSeconds = 3 * 60 * 60; // смещение на 3 часа
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Можно по-другому - указать конкретный пул сервера времени и смещение (в секундах, может быть
// изменено позже с помощью setTimeOffset()). Дополнительно можно указать интервал
// обновления (в миллисекундах, что может быть изменено с помощью setUpdateInterval()).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" ");

  timeClient.begin();
}

void loop() 
{
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());
  delay(1000);
}
