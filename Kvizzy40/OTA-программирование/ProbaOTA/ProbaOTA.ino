
// ProbaOTA.ino - попытка перепрограммировать через OTA
#include <WiFi.h>         // For connecting ESP32 to WiFi
#include <ArduinoOTA.h>   // For enabling over-the-air updates

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

// const char* ssid     = "OPPO A9 2020";
// const char* password = "b277a4ee84e8";

void setup() 
{
  // Подключиться к WiFi — по умолчанию в режиме WiFi-станции
  WiFi.begin(ssid, password); // Connect to WiFi - defaults to WiFi Station mode
   
  // Убедитесь, что Wi-Fi подключен
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  // Запускает OTA
  ArduinoOTA.begin(); 

  // Включаем последовательный порт
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("Последовательный порт работает!");

}

void loop() 
{
  // Обрабатывает запрос на обновление кода
  ArduinoOTA.handle(); 
  // All loop you're code goes here.

  // Ничего не делаем секунду
  vTaskDelay(1024/portTICK_PERIOD_MS); 

  Serial.println("Опять Последовательный порт работает дальше!");
}


