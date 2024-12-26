/** Arduino, Esp32-CAM ******************************************** Lead.h ***
 * 
 *        Выбрать накопившиеся json-сообщения о состоянии устройств контроллера 
 *            и показаниях датчиков из очереди и отправить их на страницу State 
 * 
 * v3.3.2, 23.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>

// ****************************************************************************
// *      Отправить регулярный (по таймеру) запрос контроллера на изменение   *
// *                  состояний его устройств к странице Lead                 *
// ****************************************************************************
/*
String sendLead(uint32_t iLead) 
{
   String Result="113";
   // Выполняем проверку подключения к беспроводной сети
   if ((WiFi.status() == WL_CONNECTED)) 
   {
      HTTPClient http;
      String queryString = "cycle="+String(iLead);
      String ehttp=shttp+"Lead/";
      http.begin(ehttp);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpCode = http.POST(queryString);
      // httpCode will be negative on error
      if (httpCode > 0) 
      {
         // file found at server
         if (httpCode == HTTP_CODE_OK) 
         {
            Result = http.getString();
         } 
         else 
         {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] POST... code: %d\n", httpCode);
         }
      } 
      else 
      {
         Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
   }
   return Result;
}
*/
// * Задача FreRTOS ***********************************************************
// *      Отправить регулярный (по таймеру) запрос контроллера на изменение   *
// *                  состояний его устройств к странице Lead                 *
// ****************************************************************************
void vLead(void* pvParameters) 
{
   tQueryMessage tQuery;
   String queryString = "cycle="+String(iLead);
   String ehttp=shttp+"Leadi/";
   int iTrass=0;

   for (;;)
   {
      iLead++;
      // Делаем запрос к Lead
      //String ContentPage = sendLead(iLead); 
      tQuery = postQuery(ehttp, queryString); 
      // Трассировочное сообщение в очередь
      iTrass++;
      //if (iTrass>5)
      //{
      //   iTrass=0;
         Serial.print(iLead); Serial.print("-Lead: "); Serial.println(tQuery.httpText);
      //}
      // Отмечаем флагом, что цикл задачи успешно завершен   
      fwdtLead = true;
      // Пропускаем интервал 897 мсек
      vTaskDelay(897/portTICK_PERIOD_MS); 
   }
}

// **************************************************************** State.h ***
