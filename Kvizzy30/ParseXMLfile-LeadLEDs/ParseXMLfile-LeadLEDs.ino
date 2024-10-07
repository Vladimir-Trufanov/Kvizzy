/** Arduino C/C++ ******************************* ParseXMLfile-LeadLEDs.ino ***
 * 
 * Считать и распарсить XML-файл
 * "https://kwinflatht.nichost.ru/State/ParseXMLfile-LeadLEDs.xml":
 * 
 *   <positions id="root">
 *     <name>Всем привет!</name>
 *     <sgpa id="sgpa">181</sgpa>
 *     <Led04 id="Led04">HIGH</Led04>
 *     <Led33 id="inLed33">inHIGH</Led33>
 *   </positions>
 *   
 * и поуправлять встроенными светодиодами
 * 
 * v3.1, 07.10.2024                                    Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                                Дата создания: 31.05.2024
**/

// Payment:                              "Al Thinker ESP32-CAM"
// CPU Frequency:                        "80MHz (WiFi/BT)"
// Flash Frequency:                      "80MHz"
// Flash Mode:                           "QIO"
// Partition Scheme:                     "Huge APP(3MB No OTA/1MB SPIFFS)"
// Core Debug Level:                     "Ничего"
// Erase All Flash Before Sketch Upload: "Enabled"
// Port:                                 "COM5"

// Additional links for the Board Manager: https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json

// Менеджер плат:         esp32 by Espressif Systems 3.0.1 installed
// Платы в данном пакете: ESP32 Dev Board, ESP32-S2 Dev Board, ESP32-S3 Dev Board, ESP32-C3 Dev Board, Arduino Nano ESP32.

// Подключаем библиотеки для работы с WiFi и HTTP-протоколом
#include <WiFi.h>
#include <HTTPClient.h>
// Подключаес библиотеку для работы с XML
#include <tinyxml2.h>
using namespace tinyxml2;

// Вводим имя и пароль точки доступа
//const char *ssid =     "linksystve";     // Put your SSID here
//const char *password = "x93k6kq6wf";     // Put your PASSWORD here
//const char* ssid     = "OPPO A9 2020";
//const char* password = "b277a4ee84e8";

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

// Инициируем счетчик перезапусков основного цикла
int iloop=0;

void setup() 
{
   Serial.begin(115200);
   delay(3000);
   // Подключаемся к Wi-Fi сети
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
   {
      delay(1000);
      Serial.println("Соединяемся с Wi-Fi..");
   }
   Serial.println("Соединение с Wi-Fi установлено");
   // Настраиваем пины светодиодов на выход
   pinMode(4,OUTPUT);      // "вспышка"
   pinMode(33,OUTPUT);     // "рабочий светодиод"
   // Гасим светодиоды
   Serial.println("Погасили");
   digitalWrite(33,HIGH);  // рабочий
   digitalWrite(4,LOW);    // "вспышка"
   delay(1000);
   view();
   delay(3000);
}

void loop() 
{
  // Изменяем счетчик перезапусков
  iloop=iloop+1;
  Serial.print("Перезапуск: "); Serial.println(iloop);
  // Выполняем проверку подключения к беспроводной сети
  if ((WiFi.status() == WL_CONNECTED)) 
  {
    // Создаем объект для работы с HTTP
    HTTPClient http;
    // Подключаемся к веб-странице
    //String xmlname="https://kwinflatht.nichost.ru/";
    //String xmlname="https://kwinflatht.nichost.ru/State/State.php";
    //String xmlname="http://localhost:100/State/ParseXMLfile-LeadLEDs.xml"; // Ошибка HTTP-запроса
    String xmlname="https://kwinflatht.nichost.ru/State/ParseXMLfile-LeadLEDs.xml";
    // Начинаем сеана связи 
    http.begin(xmlname);
    // Выполняем GET запрос
    int httpCode = http.GET();
    // Проверяем успешность запроса
    if (httpCode > 0) 
    {
      // Выводим ответ сервера
      String payload = http.getString();
      Serial.println("httpCode ---");
      Serial.println(httpCode);
      Serial.println("payload ----");
      Serial.println(payload);
      Serial.println("------------");
      // Переводим содержимое XML-файла в строку символов
      // через указатель для распознавания
      int length = payload.length();
      char* testDocument = new char[length + 1];  
      strcpy(testDocument,payload.c_str()); 
      // Создаём объект и распознаем XML-строку
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
      // Выбираем значение отсутствующего параметра "noname"
      Serial.println("noname -----");
      XMLElement* noname = root->FirstChildElement("noname");
      if (noname != nullptr) Serial.println(noname->GetText());
      else Serial.println("nullptr");
      Serial.println("------------");

      // Разбираем указания по светодиодам, выполняем их и проверяем
      Serial.println("Выполняем указания по светодиодам - включаем их");
      XMLElement* text_L04 = root->FirstChildElement("Led04");
      String textL04=text_L04->GetText();
      Serial.println("Led04: '"+textL04+"'");
      if (textL04 == "HIGH") digitalWrite(4,HIGH);   // зажгли "вспышку"
      else digitalWrite(4,LOW);

      XMLElement* text_L33 = root->FirstChildElement("Led33");
      String textL33=text_L33->GetText();
      Serial.println("Led33: '"+textL33+"'");
      if (textL33 == "inHIGH") digitalWrite(33,LOW); // зажгли "рабочий светодиод"
      else digitalWrite(33,HIGH);
      
      view();
    }
    else 
    {
      Serial.println("Ошибка HTTP-запроса");
    }
    // Освобождаем ресурсы микроконтроллера
    http.end();
  }

   // Гасим светодиоды
   delay(4000);
   Serial.println("Погасили");
   digitalWrite(33,HIGH);  // рабочий
   digitalWrite(4,LOW);    // "вспышка"
   delay(1000);
   view();
   delay(4000);
}

// ****************************************************************************
// *                      Проверить состояния светодиодов                     *
// ****************************************************************************
void view() 
{
  if (digitalRead(4) == HIGH) Serial.println("4-горит"); 
  else Serial.println("4-потушена");
  if (digitalRead(33) == HIGH) Serial.println("33-потушена"); 
  else Serial.println("33-горит");
}

// *********************************************  ParseXMLfile-LeadLEDs.ino ***
