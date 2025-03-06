/** Arduino, Esp32-CAM ******************************************** Lead.h ***
 * 
 *        Выбрать накопившиеся json-сообщения о состоянии устройств контроллера 
 *            и показаниях датчиков из очереди и отправить их на страницу State 
 * 
 * v3.3.4, 28.01.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>
#include <Regexp.h>

// ****************************************************************************
// *    Выбрать и обработать текущее json-сообщение из ответа страницы Lead   *
// ****************************************************************************
void match_callback(const char * match,const unsigned int length,const MatchState & ms)
{
   // Выбираем очередной найденный фрагмент
   // {"led33":[{"regim":0}]}
   String sjson = String(match);
   sjson = sjson.substring(0,length);
   
   // Обрабатываем очередной найденный фрагмент
   
   // "режим контрольного светодиода выключен"
   if (sjson==s33_MODE0) 
   {
      Serial.println(); 
      Serial.println(sjson); 
      Serial.println("Выключаем режим контрольного светодиода"); 
      Serial.println("Обновляем json-документ"); 
      
      // Отправляем сообщение на State
      String inMess=queState.Send(s33_MODE0);
      if (inMess!=tisOk) queMessa.Send(tmt_WARNING,NoSend33_MODE0,tmk_Queue);
   }
   else if (sjson==s33_LOW) 
   { 
      Serial.println(); 
      Serial.print(sjson); Serial.println(" -> match_callback.ELSE-IF"); 
   }
   else 
   { 
      Serial.println(); 
      Serial.print(sjson); Serial.println(" -> match_callback.ELSE"); 
   }
}
// ****************************************************************************
// *      Выполнить выборку всех json-сообщений из ответа страницы Lead       *
// ****************************************************************************
void getJsonLead(String httpText)
{
  unsigned long count;
  // Загружаем в буфер текст, в котором будет осуществляться поиск
  char buf[100];
  count=httpText.length();
  if (count+1>100) Serial.println("Ошибка, httpText превышает 100 символов!");
  httpText.toCharArray(buf,count+1);  
  // Создаем объект поиска соответствий
  MatchState ms(buf);
  // Формируем регулярное выражение ("соответствие") для поиска json-сообщений:
  // "<Lead><p>{"led33":[{"regim":0}]}</p></Lead>"
  const char * match="{[{\"a-z0-9:%[}%]]*}";
  // Выполняем поиск по соответствию
  count = ms.GlobalMatch(match,match_callback);
}
// * Задача FreRTOS ***********************************************************
// *      Отправить регулярный (по таймеру) запрос контроллера на изменение   *
// *                  состояний его устройств к странице Lead                 *
// ****************************************************************************
void vLead(void* pvParameters) 
{
   // Готовим запрос к странице Lead
   String ehttp=shttp+"Lead/";                   // запрос
   String queryString = "cycle="+String(iLead);  // параметры
   tQueryMessage tQuery;                         // ответ
   String httpText;                              // текст ответного сообщения
   // Зацикливаем задачу
   for (;;)
   {
      //Serial.println("*** vLead ***");
      iLead++;
      // Делаем запрос к Lead
      tQuery = postQuery(ehttp, queryString);
      // Обрабатываем успешный запрос 
      if (tQuery.httpCode == HTTP_CODE_OK) 
      {
         // Выбираем json-сообщения из ответа Lead 
         getJsonLead(tQuery.httpText);
      }
      // Реагируем на ошибку Post-запроса
      else
      {
         // Пока ничего не делаем, сообщения об ошибках отправлены в postQuery   
      }
      
      // Отмечаем флагом, что цикл задачи успешно завершен   
      fwdtLead = true;
      // Пропускаем интервал 897 мсек
      vTaskDelay(897/portTICK_PERIOD_MS); 
   }
}

// **************************************************************** Lead.h ***
