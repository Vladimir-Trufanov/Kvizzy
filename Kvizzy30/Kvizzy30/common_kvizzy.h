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
   tQueryMessage tQuery;
   tQuery.httpCode=1001;
   tQuery.httpText="1001";
   // Выполняем проверку подключения к беспроводной сети
   if ((WiFi.status() == WL_CONNECTED)) 
   {
      HTTPClient http;
      //String queryString = "cycle="+String(iLead);
      //String ehttp=shttp+"Lead/";
      http.begin(ehttp);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      tQuery.httpCode = http.POST(queryString);
      // httpCode will be negative on error
      if (tQuery.httpCode > 0) 
      {
         // file found at server
         if (tQuery.httpCode == HTTP_CODE_OK) 
         {
            tQuery.httpText = http.getString();
         } 
         else 
         {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] POST... code: %d\n", tQuery.httpCode);
            tQuery.httpText="1002";
         }
      } 
      else 
      {
         Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(tQuery.httpCode).c_str());
         tQuery.httpText="1002";
      }
      http.end();
   }
   return tQuery;
   /*
    400 => array('400 Плохой запрос', 'Запрос не может быть обработан из-за синтаксической ошибки'),
    403 => array('403 Запрещено', 'Сервер отказывает в выполнении вашего запроса'),
    404 => array('404 Не найдено', 'Запрашиваемая страница не найдена на сервере'),
    405 => array('405 Метод не допускается', 'Указанный в запросе метод не допускается для заданного ресурса'),
    408 => array('408 Время ожидания истекло', 'Ваш браузер не отправил информацию на сервер за отведенное время'),
    500 => array('500 Внутренняя ошибка сервера', 'Запрос не может быть обработан из-за внутренней ошибки сервера'),
    502 => array('502 Плохой шлюз', 'Сервер получил неправильный ответ при попытке передачи запроса'),
    504 => array('504 Истекло время ожидания шлюза', 'Вышестоящий сервер не ответил за установленное время'),
    999 => array('999 Неопределенная ошибка', 'Код ошибки HTTP/HTTPS неправильный'),

    998  = array('998 Незарегистрированная ошибка', 'Код ошибки '.$status.' незарегистрирован');
   */
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
