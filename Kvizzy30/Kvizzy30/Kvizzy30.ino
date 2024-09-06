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
      Serial.println(httpCode);
      Serial.println(payload);

      // парсим
      //char * testDocument = httpCode;
      
      char* testDocument = "<article><title>How to parser XML with TinyXML2 C++</title><description>A simple and functional tool to read the tilemaps of your games.</description><date>2022-03-04</date><time>13:40</time><tagi>28</tagi></article>";
      XMLDocument xmlDocument;
      if(xmlDocument.Parse(testDocument)!= XML_SUCCESS)
      {
         Serial.println("Error parsing");
         return; 
      };
      XMLNode * article = xmlDocument.FirstChild();
      XMLElement * tagi = article->FirstChildElement("tagi");

      int val;
      tagi->QueryIntText(&val);
      Serial.println(val);

      XMLElement* text = article->FirstChildElement("title");
      if (text != nullptr) 
      {
        Serial.println(text->GetText());
      }


      /*
      // парсинг нескольких элементов
      <?xml version="1.0" encoding="UTF-8"?>
      <article>
         <title>How to parser XML with TinyXML2 C++</title>
         <description>A simple and functional tool to read the tilemaps of your games.</description>
         <date>2022-03-04</date>
         <time>13:40</time>
         <tag>c++</tag>
      </article>
      */

      /*
      testDocument = "<article><title>How to parser XML with TinyXML2 C++</title><description>A simple and functional tool to read the tilemaps of your games.</description><date>2022-03-04</date><time>13:40</time><tag>c++</tag></article>";
      if(xmlDocument.Parse(testDocument)!= XML_SUCCESS)
      {
         Serial.println("Error 2 parsing");
         return; 
      };
      XMLNode * root = xmlDocument.FirstChild();
      XMLElement * title = root->FirstChildElement("title");

      String tTitle;
      title->GetText(&tTitle);
      */

      /*
      tinyxml2::XMLDocument doc;
      doc.LoadFile("segundo.xml");

      tinyxml2::XMLElement * p_root_element = doc.RootElement();
      tinyxml2::XMLElement * p_title = p_root_element->FirstChildElement("title");
      tinyxml2::XMLElement * p_desc = p_root_element->FirstChildElement("description");
      tinyxml2::XMLElement * p_date = p_root_element->FirstChildElement("date");
      tinyxml2::XMLElement * p_time = p_root_element->FirstChildElement("time");
      tinyxml2::XMLElement * p_tag = p_root_element->FirstChildElement("tag");
      */


      /*
      tinyxml2::XMLDocument doc;
      //doc.Parse("<greeting><text>Hello, world!</text></greeting>");
      String textDocument = "<article><title>How to parser XML with TinyXML2 C++</title><description>A simple and functional tool to read the tilemaps of your games.</description><date>2022-03-04</date><time>13:40</time><tag>c++</tag></article>";

      if (doc.Parse(textDocument&)!= XML_SUCCESS)
      {
         Serial.println("Error parsing");
         return; 
      };

      //XMLNode * article = doc.FirstChild();
      
      tinyxml2::XMLElement* text = doc.FirstChildElement("article")->FirstChildElement("title");
      if (text != nullptr) 
      {
        Serial.println(text->GetText());
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
