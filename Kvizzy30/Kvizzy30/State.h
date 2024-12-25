/** Arduino, Esp32-CAM ******************************************** State.h ***
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

String sendState(String sjson) 
{
   String Result="96";
   // Выполняем проверку подключения к беспроводной сети
   if ((WiFi.status() == WL_CONNECTED)) 
   {
      // Подключаемся к веб-странице
      //String shttp="https://doortry.ru/State/e?Com=";  // Ответ: 404
      //String shttp="https://doortryi.ru/State/?Com=";  // Ответ: -1
      //String shttp="http://doortry.ru/State/?Com=";    // Ответ: 301
      
      //String shttp="https://doortry.ru/State/?Com=";   //
      //String shttp="http://probatv.ru/?Com=State"; 
      String shttp="http://probatv.ru/kv/";
      //String shttp="http://localhost:100/kv/";
      
      //shttp += sjson;   
      //Serial.print("shttp: "); Serial.println(shttp);

      /*
      // Cоздаем объект для работы с HTTP
      HTTPClient http;
      // Делаем GET запрос
      http.begin(shttp);  
      int httpCode = http.GET();
      // Если запрос успешный получаем ответ сервера
      if (httpCode == 200) 
      {
        http.getString();
        Result = "http.getString()";
      }
      // Иначе сообщение о коде ошибки
      else Result = String(httpCode);
      // Освобождаем ресурсы микроконтроллера
      http.end();
      */

      HTTPClient http;
      String queryString = "temperature=26&humidity=70";
      http.begin(shttp);  
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

        // Если разрешено, трассируем память контроллера
        #ifdef tmr_TRACEMEMORY
         // Получаем полный размер кучи в памяти
         printf("Общий размер ВСТРОЕННОЙ памяти:     %u\n", ESP.getHeapSize());
         // Количество доступной кучи в памяти
         printf("Оставшаяся доступная память в куче: %u\n", ESP.getFreeHeap());
         // Самый низкий уровень свободной кучи с момента загрузки
         printf("Минимальная свободная с загрузки:   %u\n", ESP.getMinFreeHeap());
         // Размер общей кучи SPI PSRAM
         printf("Общий размер SPI PSRAM:             %u\n", ESP.getPsramSize());
         // Количество свободной PSRAM
         printf("Количество свободной PSRAM:         %d\n", ESP.getFreePsram());
         // Минимальный размер свободной памяти в SPI RAM
         printf("Минимум свободной SPI PSRAM:        %d\n", ESP.getMinFreePsram());
         // Размер самого большого блока PSRAM, который может быть выделен
         printf("Самый большой блок для выделения:   %d\n", ESP.getMaxAllocPsram());
      #endif


      /*
      http.begin(shttp);
      //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpCode = http.GET();
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
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
         }
      } 
      else 
      {
         Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
      */
   }
   return Result;
}
// * Задача FreRTOS ***********************************************************
// *   Выбрать накопившиеся json-сообщения о состоянии устройств контроллера  *
// *     и показаниях датчиков из очереди и отправить их на страницу State    *
// ****************************************************************************
void vState(void* pvParameters) 
{
   for (;;)
   {
      /*
      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (iCreateSit == loopingLed33) MimicMCUhangEvent("Led33");  
      */ 

      String jstr="&cjson=";
      String sjson="95";
      jstr +=sjson;
           
      // Отправляем json-строку на сайт
      String ContentPage = sendState(jstr); 
      
      iAll++;
      Serial.print(iAll); Serial.print(": ");
      Serial.println(ContentPage);

      // Отмечаем флагом, что цикл задачи успешно завершен   
      fwdtState = true;
      // Пропускаем интервал 386 мсек
      //iState++; 
      //if (iState>5)
      //{
      //  iState=0;
      //  vTaskDelay(60000/portTICK_PERIOD_MS); 
      //}
      //else 
      vTaskDelay(2986/portTICK_PERIOD_MS); 
   }
}

// **************************************************************** State.h ***
