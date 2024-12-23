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
   String Result;
   // Выполняем проверку подключения к беспроводной сети
   if ((WiFi.status() == WL_CONNECTED)) 
   {
      // Cоздаем объект для работы с HTTP
      HTTPClient http;
      // Подключаемся к веб-странице
      //String shttp="https://doortry.ru/State/e?Com=";  // Ответ: 404
      //String shttp="https://doortryi.ru/State/?Com=";  // Ответ: -1
      //String shttp="http://doortry.ru/State/?Com=";    // Ответ: 301
      
      //String shttp="https://doortry.ru/State/?Com=";   //
      String shttp="http://probatv.ru/?Com=State"; 
      
      //shttp += sjson;   
      Serial.print("shttp: "); Serial.println(shttp);

      // Делаем GET запрос
      http.begin(shttp);  
      int httpCode = http.GET();
      // Если запрос успешный получаем ответ сервера
      if (httpCode == 200) Result = http.getString();
      // Иначе сообщение о коде ошибки
      else Result = String(httpCode);
      // Освобождаем ресурсы микроконтроллера
      http.end();
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
      
      Serial.print("Ответ: ");
      Serial.println(ContentPage);

      // Отмечаем флагом, что цикл задачи успешно завершен   
      fwdtState = true;
      // Пропускаем интервал 386 мсек
      vTaskDelay(386/portTICK_PERIOD_MS); 
   }
}

// **************************************************************** State.h ***
