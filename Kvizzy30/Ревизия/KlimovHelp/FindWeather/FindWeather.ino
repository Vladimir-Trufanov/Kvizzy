#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

// Москва
// https://api.openweathermap.org/data/2.5/weather?q=Moscow,ru,pt&APPID=080b7a11c05216a4b86317b92370b484";
// Вилга
// https://api.openweathermap.org/data/2.5/weather?lat=61.7019&lon=34.1545&appid=080b7a11c05216a4b86317b92370b484

//const String endpoint = "https://api.openweathermap.org/data/2.5/weather?q=Moscow,ru,pt&APPID=";
const String endpoint = "https://api.openweathermap.org/data/2.5/weather?lat=61.7019&lon=34.1545&appid=";
const String key = "080b7a11c05216a4b86317b92370b484";
const String chttp = "https://api.openweathermap.org/data/2.5/weather?lat=61.7019&lon=34.1545&appid=080b7a11c05216a4b86317b92370b484";
//                        https://api.openweathermap.org/data/2.5/weather?lat=61.70&lon=34.15&appid=080b7a11c05216a4b86317b92370b484
bool first=true;

void setup() 
{
  Serial.begin(115200);
  // делаем небольшую задержку на открытие монитора порта
  delay(1000);
  // подключаемся к Wi-Fi сети
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Соединяемся с Wi-Fi..");
  }
  Serial.println("Соединение с Wi-Fi установлено");
}

void loop() 
{
  if (first==true)
  {
    Serial.println("first");
    // выполняем проверку подключения к беспроводной сети
    if ((WiFi.status() == WL_CONNECTED)) 
    { 
      // Check the current connection status
      // создаем объект для работы с HTTP
      HTTPClient http;
      // подключаемся к веб-странице OpenWeatherMap с указанными параметрами
      // http.begin(endpoint + key);
      http.begin(chttp);
      int httpCode = http.GET();  // Делаем запрос
      // проверяем успешность запроса
      if (httpCode > 0) 
      { 
        //Check for the returning code
        // выводим ответ сервера
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
        first=false;
      }
      else 
      {
         Serial.println("Ошибка HTTP-запроса");
      }
      http.end(); // освобождаем ресурсы микроконтроллера
      delay(3000);
    }
  }
}
