/** Arduino, Esp32-CAM **************************************** kviStream.h ***
 * 
 *               Сделать фото и отправить Base24 изображения на страницу Stream  
 * 
 * v4.0.0, 28.03.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 26.02.2025
 *
**/

#pragma once            
#include <Arduino.h>

/*
time_t nTime;      // время отправки фрэйма (секунда с начала эпохи)
time_t nTimeOld;   // время отправки предыдущего фрэйма
int nFrame=0;      // номер кадра в секунде
*/

// * Задача FreRTOS ***********************************************************
// *           Сделать фото и отправить Base24 изображения на страницу Stream *
// ****************************************************************************
void vStream(void* pvParameters) 
{
  /*
  // Готовим запрос к странице Stream
  String ehttp=shttp+"Stream/";  // запрос
  tQueryMessage tQuery;          // ответ
  String httpText;               // текст ответного сообщения
  // Инициируем метки времени
  time(&nTime);                  // время отправки фрэйма (секунда с начала эпохи)
  time(&nTimeOld);               // время отправки предыдущего фрэйма
  */
  // Зацикливаем задачу
  for (;;)
  {
    Serial.println("*** vStream ***");
    /*
    #ifdef tmr_STREAM
      // Пересчитываем время и номер кадра
      time(&nTime); 
      if (nTime==nTimeOld) nFrame=nFrame+1;
      else {nTimeOld=nTime; nFrame=0;}
      // Включаем кадр в запрос  
      //String frame="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";
      String frame="Это Frame!";
      String queryString = "src="+frame;      
      // Добавляем время с начала эпохи в параметры
      String stime="&time="+String(nTime);
      queryString=queryString+stime;
      // Добавляем номер кадра в параметры
      String sframe="&frame="+String(nFrame);
      queryString=queryString+sframe;
      // Делаем запрос к Stream
      tQuery = postQuery(ehttp, queryString);
      // Обрабатываем успешный запрос 
      if (tQuery.httpCode == HTTP_CODE_OK) 
      {
        // Выбираем json-сообщения из ответа Stream 
        // getJsonLead(tQuery.httpText);
        Serial.println(tQuery.httpText);
      }
      // Реагируем на ошибку Post-запроса
      else
      {
        Serial.print("vStream: ");
        Serial.println(tQuery.httpCode);
        // Пока ничего не делаем, сообщения об ошибках отправлены в postQuery   
      }
    #endif  
    */
    // Отмечаем флагом, что цикл задачи успешно завершен   
    fwdtStream = true;
    // Пропускаем интервал 2971 мсек
    vTaskDelay(2971/portTICK_PERIOD_MS); 
  }
}

// ************************************************************ kviStream.h ***
