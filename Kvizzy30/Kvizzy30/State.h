/** Arduino, Esp32-CAM ******************************************** State.h ***
 * 
 *           ---Обеспечить определение состояния контрольного светодиода ESP32-CAM
 *           ---    ("горит - не горит") и передачу данных на страницу сайта State
 * 
 * v1.2, 08.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#ifndef State_
#define State_
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
      // http://localhost:100/State/?Com={%22nicctrl%22:%22myjoy%22,%22led33%22:[{%22typedev%22:%22inLed%22,%22status%22:%22inHIGH%22}]}
      // http.begin("https://doortry.ru/State/?Com="+"Privet");  
      
      //String shttp="https://doortry.ru/State/?Com=Privet";
      String shttp="https://doortry.ru/State/?Com=";
      //shttp += "Privetik";   
      shttp += sjson;   
      Serial.print("shttp: ");
      Serial.print(shttp);
      Serial.println("***");


      //shttp="https://doortry.ru/State/?Com=Privetik";
      http.begin(shttp);  

      // Делаем GET запрос
      int httpCode = http.GET();
      // Проверяем успешность запроса
      if (httpCode == 200) 
      {
         // Получаем ответ сервера
         Result = http.getString();
         //Serial.println(httpCode);
         //Serial.println(ContentPage);
      }
      else 
      {
         Result = String(httpCode);
         //Serial.println("Ошибка HTTP-запроса");
      }
      // Освобождаем ресурсы микроконтроллера
      http.end();
   }
   return Result;
}

#endif

// **************************************************************** State.h ***
