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
#include <ArduinoJson.h>

// ****************************************************************************
// *    Выбрать и обработать текущее json-сообщение из ответа страницы Lead   *
// ****************************************************************************
void match_callback(const char * match,const unsigned int length,const MatchState & ms)
{
  // Выбираем очередной найденный фрагмент
  // {"led33":[{"regim":0}]}
  String sjson = String(match);
  sjson = sjson.substring(0,length);
  Serial.print("sjson: "); Serial.println(sjson); 

  //JsonDocument doc;
  //deserializeJson(doc, sjson);

  /*
  // {"lead":[{"led4":[{"light":25,"time":1996}]},{"intrv":[{"mode4":6900,"img":1001,"tempvl":3003,"lumin":2002,"bar":5005}]}]}

  String lead=doc["lead"];
  Serial.print("lead: "); Serial.println(lead);
  
  // lead: [{"led4":[{"light":25,"time":1996}]},{"intrv":[{"mode4":6900,"img":1001,"tempvl":3003,"lumin":2002,"bar":5005}]}]
  String led4=doc["lead"][0]["led4"];
  Serial.print("led4: "); Serial.println(led4);
  // led4: [{"light":25,"time":1996}]
  int light = doc["lead"][0]["led4"][0]["light"];
  Serial.print("light: "); Serial.println(light);

  // lead: [{"led4":[{"light":25,"time":1996}]},{"intrv":[{"mode4":6900,"img":1001,"tempvl":3003,"lumin":2002,"bar":5005}]}]
  String intrv=doc["lead"][1]["intrv"];
  Serial.print("intrv: "); Serial.println(intrv);

  //int mode4 = doc["lead"][0]["intrv"][0]["mode4"];
  //Serial.print("mode4: "); Serial.println(mode4);
  */

  // {{"led4":{"light":25,"time":1996}},{"intrv":{"mode4":6900,"img":1001,"tempvl":3003,"lumin":2002,"bar":5005}}
  sjson = '{{"led4":{"light":25,"time":1996}},{"intrv":{"mode4":6900,"img":1001,"tempvl":3003,"lumin":2002,"bar":5005}}';
  Serial.print("sjson: "); Serial.println(sjson); 

  JsonDocument doc;
  deserializeJson(doc, sjson);


 
   // Обрабатываем очередной найденный фрагмент
   
   // "режим контрольного светодиода выключен"
   /*
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
   */
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

// ****************************************************************************
// *      Выполнить выборку всех json-сообщений из ответа страницы Lead       *
// ****************************************************************************
void Deser(String httpText)
{

  
  
  
  
  
  // Готовим ответ для парс
  int str_len = httpText.length()+1; 
  char json[str_len];
  httpText.toCharArray(json, str_len);
  // Трассируем ответ от Lead
  Serial.print("json: "); Serial.println(json);

  // Создаем объект поиска соответствий
  MatchState ms(json);
  // Формируем регулярное выражение ("соответствие") для поиска json-сообщений:
  // <Lead>{"lead":[{"led4":[{"light":25,"time":1996}]},{"intrv":[{"mode4":6900,"img":1001,"tempvl":3003,"lumin":2002,"bar":5005}]}]}</Lead>
  const char * match="{[{\"a-z0-9:,%[}%]]*}";
  // Выполняем поиск по соответствию
  int count = ms.GlobalMatch(match,match_callback);
  Serial.print("count: "); Serial.println(count);


  /*
  charBuf[stringOne.length()+1] –
  
  char json[512] = httpText;
  JsonDocument doc;
//deserializeJson(doc, json);
*/


/*
char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

JsonDocument doc;
deserializeJson(doc, json);

const char* sensor = doc["sensor"];
long time          = doc["time"];
double latitude    = doc["data"][0];
double longitude   = doc["data"][1];
*/



}

// * Задача FreRTOS ***********************************************************
// *      Отправить регулярный (по таймеру) запрос контроллера на изменение   *
// *                  состояний его устройств к странице Lead                 *
// ****************************************************************************
void vLead(void* pvParameters) 
{
  // Зацикливаем задачу
  for (;;)
  {
    //Serial.println("*** vLead ***");
    #ifdef tmr_LEAD
      // В vLead конкурируем за Http-запросы. Будем до 3 секунд ждать освобождения запроса.
      // По опыту от 24.04.2025 на запрос к probatv.ru может уйти до полутора секунд.
      // Если захватить блокировку не получается в течение этого периода времени, прекращаем попытки 
      // и вместо отработки задачи выводим сообщение. 
      if (xSemaphoreTake (HttpMutex, (3000 * portTICK_PERIOD_MS))) 
      { 
        /*
        // Имитируем зависание микроконтроллера с помощью опознанного числа,
        // принятого в последовательном порту
        if (iCreateSit == loopingLed4) MimicMCUhangEvent("Led4");   
        */
        // Передаём запрос на изменение состояний
        Led4State(s4_MODE);

        // Готовим запрос к странице Lead
        String ehttp=urlHome+"/Lead40/";
        // Изменяем значение счетчика и включаем его в параметр запроса к странице State
        iLead=incUINT32T(iLead);
        String queryString = "cycle="+String(iLead);    
        // Готовим структуру для ответа
        tQueryMessage tQuery;                              
        // Включаем в параметр запроса json-сообщение
        String sjson="&sjson="+s_COMMON;
        queryString=queryString+sjson;
        tQuery = postQuery(ehttp, queryString);
        // Обрабатываем успешный запрос 
        if (tQuery.httpCode == HTTP_CODE_OK) Deser(tQuery.httpText);
        // Иначе реагируем на ошибку Post-запроса
        else
        {
          Serial.print("Ошибка Post-запроса от Lead40: "); Serial.println(tQuery.httpCode);
        }
        // Освобождаем мьютекс
        xSemaphoreGive(HttpMutex);  
      }
      else 
      {  
        Serial.println("vLead: HttpMutex не был захвачен!");
      }
    #endif  
    // Отмечаем флагом, что цикл задачи успешно завершен   
    fwdtLead = true;
    // Пропускаем интервал 897 мсек
    vTaskDelay(997/portTICK_PERIOD_MS); 
  }
}

// **************************************************************** Lead.h ***
