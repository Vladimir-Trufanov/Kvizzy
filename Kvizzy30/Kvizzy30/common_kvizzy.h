/** Arduino, Esp32-CAM ************************************ common_kvizzy.h ***
 * 
 * Определить общие функции нижнего уровня умного хозяйства на двух светодиодах
 * 
 * v1.1.1, 24.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>

// * Задача FreRTOS ***********************************************************
// *      Выбрать из очереди и вывести сообщения в последовательный порт      *
// ****************************************************************************
void vPrint(void* pvParameters) 
{
   for (;;)
   {
      queMessa.PostAll();
      // Отмечаем флагом, что цикл задачи успешно завершен   
      fwdtPrint = true;
      vTaskDelay(439/portTICK_PERIOD_MS); 
   }
}
// ****************************************************************************
// *                     Выполнить POST-запрос к странице сайта               *
// ****************************************************************************
tQueryMessage postQuery(String ehttp, String queryString) 
{
   String inMess;
   tQueryMessage tQuery;
   tQuery.httpCode=1001;
   tQuery.httpText="1001";
   // Выполняем проверку подключения к беспроводной сети
   if ((WiFi.status() == WL_CONNECTED)) 
   {
      HTTPClient http;
      http.begin(ehttp);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      tQuery.httpCode = http.POST(queryString);
      if (tQuery.httpCode > 0) 
      {
         // Если запрос успешно отправлен
         if (tQuery.httpCode == HTTP_CODE_OK) 
         {
            tQuery.httpText = http.getString();
         }
         // Если ошибка после того, как HTTP-заголовок был отправлен
         // и заголовок ответа сервера был обработан
         else 
         {
            inMess=queMessa.Send(tmt_WARNING,TaskNoQueue,tmk_HTTP);
            if (inMess!=isOk) Serial.println(inMess); 
            inMess=queMessa.Send(tmt_WARNING,TaskNoQueue);
            if (inMess!=isOk) Serial.println(inMess); 
            inMess=queMessa.Send(tmt_WARNING,tQuery.httpCode,tmk_HTTP);
            if (inMess!=isOk) Serial.println(inMess); 
            //Serial.printf("[HTTP] POST... code: %d\n", tQuery.httpCode);
            tQuery.httpText=inMess;
         }
      }
      // Если ошибка при отправке POST-запроса 
      else 
      {
         Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(tQuery.httpCode).c_str());
         tQuery.httpText=http.errorToString(tQuery.httpCode);
      }
      http.end();
   }
   // Если "Нет подключения к WiFi перед передачей POST-запроса"
   else
   {
      inMess=queMessa.Send(tmt_WARNING,http997,tmk_HTTP);
      if (inMess!=isOk) Serial.println(inMess); 
   }
   return tQuery;
}

// ****************************************************************************
// *                      --Преобразовать json-документ в строку              *
// *                      --https://arduinojson.org/                          *
// ****************************************************************************
String jison1()
{
   String str = "***";
   return str;
}


// Может пригодится!!!

// Сделать определитель типов:
#define tstr "tstr"
#define tchr "tchr"
#define tint "tint"
String types(String a) {return tstr;}
String types(char *a)  {return tchr;}
String types(int a)    {return tint;}

// ******************************************************** common_kvizzy.h ***
