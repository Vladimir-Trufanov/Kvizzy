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
    //http://kwinflatht.nichost.ru/

    //String xmlname="https://kwinflat.ru/Dacha/Proba.xml";
    String xmlname="http://kwinflatht.nichost.ru/";
    http.begin(xmlname);
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

      /*
      // Распознаем XML-файл
      int length = payload.length();
      char* testDocument = new char[length + 1];  
      strcpy(testDocument,payload.c_str()); 

      XMLDocument xmlDocument;
      if (xmlDocument.Parse(testDocument)!= XML_SUCCESS)
      {
         Serial.print("Ошибка распознавания файла: ");
         Serial.println(xmlname);
         return; 
      };

      // Отмечаем успешное распознавание xml-файла
      Serial.print(xmlname);
      Serial.println(" - файл распознан!");

      // Подключаем к разбору корневой тег 
      XMLNode* root = xmlDocument.FirstChild();

      // Выбираем значение параметра "sgpa"
      Serial.println("sgpa -------");
      XMLElement* sgpa = root->FirstChildElement("sgpa");
      int val;
      sgpa->QueryIntText(&val);
      Serial.println(val);
      Serial.println("------------");

      // Выбираем значение параметра "name"
      Serial.println("name -------");
      XMLElement* text = root->FirstChildElement("name");
      if (text != nullptr) Serial.println(text->GetText());
      else Serial.println("nullptr");
      Serial.println("------------");

      // Выбираем значение параметра "noname"
      Serial.println("noname -----");
      XMLElement* noname = root->FirstChildElement("noname");
      if (noname != nullptr) Serial.println(noname->GetText());
      else Serial.println("nullptr");
      Serial.println("------------");
      */
    }
    else 
    {
      Serial.println("Ошибка HTTP-запроса");
    }
    // освобождаем ресурсы микроконтроллера
    http.end();
  }
  delay(3000);
}
