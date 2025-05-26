/** Arduino, Esp32-CAM ************************************ common_kvizzy.h ***
 * 
 *                          Определить общие функции исполнительного приложения 
 *                                    умного хозяйства на контроллере ESP32-CAM
 * 
 * v4.4.0, 26.05.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>

// ****************************************************************************
// *        Инициировать параметры действующего режима работы вспышки и       *
// *                интервалов подачи сообщений от контроллера                *
// ****************************************************************************
void iniPMem(Preferences Prefs) 
{
  // Извлекаем постоянные данные
  Prefs.begin("KvizzyPrefs", false);
  jlight=Prefs.getInt("jlight",10);
  inlight=Prefs.getInt("inlight",10);
  jtime=Prefs.getInt("jtime",2000);
  intime=Prefs.getInt("intime",2000);

  jmode4=Prefs.getInt("jmode4",7007);
  mode4=Prefs.getInt("mode4",7007);
  jimg=Prefs.getInt("jimg",1001);
  img=Prefs.getInt("img",1001);
  jtempvl=Prefs.getInt("jtempvl",3003);
  tempvl=Prefs.getInt("tempvl",3003);
  jlumin=Prefs.getInt("jlumin",2002);
  lumin=Prefs.getInt("lumin",2002);
  jbar=Prefs.getInt("jbar",5005);
  bar=Prefs.getInt("bar",5005);
  Prefs.end();

  // Рассчитываем времена свечения и несвечения вспышки
  nLight=jtime*jlight/100;  // 2000*10/100=200
  nNoLight=jtime-nLight;    // 2000-200=1800
  nLight=nLight*1000;       // 200000 мкс
  nNoLight=nNoLight*1000;   // 1800000 мкс
}

// ****************************************************************************
// *                     Выполнить POST-запрос к странице сайта               *
// ****************************************************************************
tQueryMessage postQuery(String urlPage, String queryString) 
{
  tQueryMessage tQuery; tQuery.httpCode=1001; tQuery.httpText="Только инициализировано";
  // Выполняем проверку подключения к беспроводной сети
  if ((WiFi.status() == WL_CONNECTED)) 
  {
    HTTPClient http;
    // Назначаем страницу запроса: http.begin("http://probatv.ru/Stream40/");
    String ehttp=urlHome+"/"+urlPage+"/";
    http.begin(ehttp);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int last = millis();     // текущее время (уходящее в прошлое)
    tQuery.httpCode = http.POST(queryString); 
 
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
      // Для трассировки формируем URI
      String URI=ehttp+"?"+queryString;
      // Если запрос успешно отправлен
      if (tQuery.httpCode == HTTP_CODE_OK) 
      {
        tQuery.httpText=http.getString();
        // Трассируем успешный запрос и ответ
        /*
        if (urlPage==urlLead)
        {
          Serial.print("Lead-запрос: "); Serial.println(URI);
          Serial.print("Lead- ответ: "); Serial.println(tQuery.httpText);
          Serial.print("Lead- время: "); Serial.print(millis()-last); Serial.println(" (мс)");
        }
        */
        if (urlPage==urlState)
        {
          Serial.print("State-запрос: "); Serial.println(URI);
          Serial.print("State- ответ: "); Serial.println(tQuery.httpText);
          Serial.print("State- время: "); Serial.print(millis()-last); Serial.println(" (мс)");
        }
      }
      // Если ошибка после того, как HTTP-заголовок был отправлен
      // и заголовок ответа сервера был обработан
      else 
      {
        // Выводим страницу и часть параметров запроса перед ошибкой 
        Serial.print("Запрос: "); Serial.println(URI);
        tQuery.httpText=queMessa.Send(tmt_WARNING,m1001,queryString.substring(0,32));
        if (tQuery.httpText!=isOk) Serial.println(tQuery.httpText);  
        // Если сообщение о ненайденной странице, указываем её
        if (tQuery.httpCode==404) tQuery.httpText=queMessa.Send(tmt_WARNING,tQuery.httpCode,ehttp);
        // Иначе выводим указанное сообщение об ошибке
        else tQuery.httpText=queMessa.Send(tmt_WARNING,tQuery.httpCode,tmk_HTTP);
        if (tQuery.httpText!=isOk) Serial.println(tQuery.httpText); 
      }
    }
    // Если ошибка при отправке POST-запроса
    //    Ошибка POST-запроса: "read Timeout"       - "истекло время ожидания чтения"
    //    Ошибка POST-запроса: "connection refused" - "В соединении отказано"
    else 
    {
      tQuery.httpText=http.errorToString(tQuery.httpCode);
      Serial.printf("Ошибка POST-запроса: %s\n", tQuery.httpText.c_str());
    }
    http.end();
  }
  // Если "Нет подключения к WiFi перед передачей POST-запроса"
  else
  {
    tQuery.httpCode=http997;
    tQuery.httpText=queMessa.Send(tmt_WARNING,http997,tmk_HTTP);
    if (tQuery.httpText!=isOk) Serial.println(tQuery.httpText); 
  }
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
