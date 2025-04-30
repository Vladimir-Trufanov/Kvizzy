/** Arduino, Esp32-CAM ************************************ common_kvizzy.h ***
 * 
 *                          Определить общие функции исполнительного приложения 
 *                                    умного хозяйства на контроллере ESP32-CAM
 * 
 * v4.0.1, 30.04.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>

// ****************************************************************************
// *                     Выполнить POST-запрос к странице сайта               *
// ****************************************************************************
tQueryMessage postQuery(String ehttp, String queryString) 
{
  //Serial.print("Передаём запрос: "); Serial.print(ehttp); Serial.println(queryString);
  String inMess;
  tQueryMessage tQuery;
  tQuery.httpCode=1001;
  tQuery.httpText="1001";
  // Выполняем проверку подключения к беспроводной сети
  if ((WiFi.status() == WL_CONNECTED)) 
  {
    HTTPClient http;
    http.begin(ehttp);
    //http.begin("http://probatv.ru/Stream40/");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int last = millis();     // текущее время (уходящее в прошлое)
    tQuery.httpCode = http.POST(queryString); 
    Serial.print("Время ответа (мс): "); Serial.println(millis() - last);
 
    if (tQuery.httpCode > 0) 
    {
      // Не получилось выбрать заголовки
      /*
      for(int i = 0; i< http.headers(); i++)
      {
        Serial.print("http.header(i) = ");
        Serial.println(i);
        Serial.println(http.header(i));
      }
      */
      // Если запрос успешно отправлен
      if (tQuery.httpCode == HTTP_CODE_OK) 
      {
        inMess = http.getString();
        Serial.print("Запрос успешно отправлен: "); Serial.println(inMess);
        //Serial.print("queryString: "); Serial.println(queryString);
      }
      // Если ошибка после того, как HTTP-заголовок был отправлен
      // и заголовок ответа сервера был обработан
      else 
      {
        Serial.println("запрос c ошибкой");
        // Если сообщение о ненайденной странице, указываем её
        if (tQuery.httpCode==404) 
        {
          inMess=queMessa.Send(tmt_WARNING,tQuery.httpCode,ehttp);
        }
        // Иначе выводим указанное сообщение
        else inMess=queMessa.Send(tmt_WARNING,tQuery.httpCode,tmk_HTTP);
        if (inMess!=isOk) Serial.println(inMess); 
      }
    }
    // Если ошибка при отправке POST-запроса
    //    Ошибка POST-запроса: "read Timeout"       - "истекло время ожидания чтения"
    //    Ошибка POST-запроса: "connection refused" - "В соединении отказано"
    else 
    {
      inMess=http.errorToString(tQuery.httpCode);
      Serial.printf("Ошибка POST-запроса: %s\n", inMess.c_str());
    }
    http.end();
  }
  // Если "Нет подключения к WiFi перед передачей POST-запроса"
  else
  {
    tQuery.httpCode=http997;
    inMess=queMessa.Send(tmt_WARNING,http997,tmk_HTTP);
    if (inMess!=isOk) Serial.println(inMess); 
  }
  // Вкладываем ответное сообщение в возвращаемую структуру
  tQuery.httpText=inMess;
  return tQuery;
}

// ****************************************************************************
// *        Инкрементировать значение счетчика с контролем максимума          *
// ****************************************************************************
uint32_t incUINT32T(uint32_t value)
{
  value++;
  if (value>4294967290) value=0;
  return value;  
}

/*
// Может пригодится!!!

// Сделать определитель типов:
#define tstr "tstr"
#define tchr "tchr"
#define tint "tint"
String types(String a) {return tstr;}
String types(char *a)  {return tchr;}
String types(int a)    {return tint;}

// Перевести массив char в String и обратно
void schastr()
{
   // Определяем структуру изменяемого сообщения
   struct AMessage
   {
      int  ucSize;        // Длина сообщения (максимально 256 байт)
      char ucData[256];   // Текст сообщения
   }  xMessage;
   
   String temp = "Всем привет!";
   strcpy(xMessage.ucData, temp.c_str());
   xMessage.ucSize = 0;
   while (xMessage.ucData[xMessage.ucSize]>0) 
   {
      xMessage.ucSize++;
   }
   Serial.println(temp);
   Serial.println(types(temp));
   Serial.println(xMessage.ucData);
   Serial.println(types(xMessage.ucData));
   Serial.println(xMessage.ucSize);
   Serial.println(types(xMessage.ucSize));
   
   String temp1=String(xMessage.ucData);
   Serial.println(temp1);
   Serial.println(temp1.length());
   Serial.println("-----");
}
*/

// ******************************************************** common_kvizzy.h ***
