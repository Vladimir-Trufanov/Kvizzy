// Подключаем библиотеку для работы с HTTP-протоколом
#include <WiFi.h>
#include <HTTPClient.h>

#include <tinyxml2.h>
using namespace tinyxml2;

// Вводим имя и пароль точки доступа
//const char *ssid =     "linksystve";     // Put your SSID here
//const char *password = "x93k6kq6wf";     // Put your PASSWORD here

//const char* ssid     = "OPPO A9 2020";
//const char* password = "b277a4ee84e8";

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

int iloop=0;

void setup() 
{
  Serial.begin(115200);
  // делаем небольшую задержку на открытие монитора порта
  delay(5000);
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
  iloop=iloop+1;
  Serial.print("iloop = ");
  Serial.println(iloop);
  
  // выполняем проверку подключения к беспроводной сети
  if ((WiFi.status() == WL_CONNECTED)) 
  {
    // создаем объект для работы с HTTP
    HTTPClient http;
    // подключаемся к веб-странице
    //http.begin("https://doortry.ru/stihi/sorevnovanie-s-hakerami");
    //http.begin("https://kwinflat.ru/Dacha/");
    //http.begin("https://kwinflat.ru/DachaXML/");
    http.begin("https://kwinflat.ru/Dacha/result.xml");
    // делаем GET запрос
    int httpCode = http.GET();
    // проверяем успешность запроса
    if (httpCode > 0) 
    {
      // выводим ответ сервера
      String payload = http.getString();
      Serial.println("httpCode ---");
      Serial.println(httpCode);
      Serial.println("payload ----");
      Serial.println(payload);
      Serial.println("------------");

      int length = payload.length();
      char* testDocument = new char[length + 1];  
      strcpy(testDocument,payload.c_str()); 

      XMLDocument xmlDocument;
      if (xmlDocument.Parse(testDocument)!= XML_SUCCESS)
      {
         Serial.println("Error parsing");
         return; 
      };

      XMLNode* istudent = xmlDocument.FirstChild();

      Serial.println("tagi -------");
      XMLElement* tagi = istudent->FirstChildElement("tagi");
      int val;
      tagi->QueryIntText(&val);
      Serial.println(val);
      Serial.println("------------");

      Serial.println("ititle -----");
      /*
      try
      {
      */
      XMLElement* text = istudent->FirstChildElement("ititle");
      if (text != nullptr) Serial.println(text->GetText());
      else Serial.println("text=nullptr");
      /*
      }
      catch (const char* error_message)
      {
         Serial.println("error_message");
         iloop=iloop+100;
      }
      */
      Serial.println("------------");

      Serial.println("texti ------");
      XMLElement* texti = istudent->FirstChildElement("title");
      if (texti != nullptr) Serial.println(texti->GetText());
      else Serial.println("texti=nullptr");
      Serial.println("------------");

      /*
      Serial.println("title ------");
      XMLElement* text = istudent->FirstChildElement("title");
      if (text != nullptr) Serial.println(text->GetText());
      Serial.println("------------");
      */

      /*
      try
      {
         XMLNode* istudent = xmlDocument.FirstChild();
      }
      catch (const char* error_message)
      {
         Serial.println("error_message");
      }
      */
    }
    else 
    {
      Serial.println("Ошибка HTTP-запроса");
    }
    // освобождаем ресурсы микроконтроллера
    http.end();
  }
  delay(10000);
}
