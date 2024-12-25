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
// *     Отправить http-запрос с json-строкой на State и вернуть ответ        *
// *               (если ответ сервера не 200, вернуть ошибку)                *
// ****************************************************************************

String sendLead(String sjson) 
{
   String Result="113";
   // Выполняем проверку подключения к беспроводной сети
   if ((WiFi.status() == WL_CONNECTED)) 
   {
      String shttp="http://probatv.ru/Lead/";

      HTTPClient httpl;
      String queryString = "temperature=26&humidity=70";
      httpl.begin(shttp);  
      httpl.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httplCode = httpl.POST(queryString);
      // httpCode will be negative on error
      if (httplCode > 0) 
      {
         // file found at server
         if (httplCode == HTTP_CODE_OK) 
         {
            Result = httpl.getString();
         } 
         else 
         {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] POST... code: %d\n", httplCode);
         }
      } 
      else 
      {
         Serial.printf("[HTTP] POST... failed, error: %s\n", httpl.errorToString(httplCode).c_str());
      }
      httpl.end();
   }
   return Result;
}
// * Задача FreRTOS ***********************************************************
// *   Выбрать накопившиеся json-сообщения о состоянии устройств контроллера  *
// *     и показаниях датчиков из очереди и отправить их на страницу State    *
// ****************************************************************************
void vLead(void* pvParameters) 
{
   for (;;)
   {
      String jlstr="&cjson=";

      // Отправляем json-строку на сайт
      String ContentPage = sendLead(jlstr); 
      
      Serial.print("Lead: ");
      Serial.println(ContentPage);

      // Отмечаем флагом, что цикл задачи успешно завершен   
      fwdtLead = true;
      // Пропускаем интервал 386 мсек
      //iState++; 
      //if (iState>5)
      //{
      //  iState=0;
      //  vTaskDelay(60000/portTICK_PERIOD_MS); 
      //}
      //else 
      vTaskDelay(897/portTICK_PERIOD_MS); 
   }
}

// **************************************************************** State.h ***
