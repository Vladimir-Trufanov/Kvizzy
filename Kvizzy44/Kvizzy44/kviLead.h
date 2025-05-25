/** Arduino, Esp32-CAM ****************************************** kviLead.h ***
 * 
 *                    Выполнить запрос управляющих изменений на страницу Lead40
 * 
 * v4.4.0, 25.05.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>
#include <Regexp.h>
#include <ArduinoJson.h>

// ****************************************************************************
// *    Выбрать и разобрать текущее json-сообщение из ответа страницы Lead,   *
// *                 сохранить значения в постоянной памяти                   *
// ****************************************************************************
void match_callback(const char * match,const unsigned int length,const MatchState & ms)
{
  // Назначаем промежуточную величину выбираемого параметра, открываемсвойства
  int value = 0;       
  Prefs.begin("KvizzyPrefs", false);
  // Выбираем очередной найденный фрагмент
  // {"led4":{"light":25,"time":1996},"intrv":{"mode4":6900,"img":1001,"tempvl":3003,"lumin":2002,"bar":5005}}
  String sjson = String(match);
  sjson = sjson.substring(0,length);
  // Трассируем выбранный json
  // Serial.print("sjson: "); Serial.println(sjson); 

  JsonDocument doc;
  deserializeJson(doc, sjson);

  // Определяемся и обрабатываем команду по режиму вспышки
  String led4=doc["led4"];
  if (led4 != "null")
  {
    value = doc["led4"]["light"];
    Prefs.putInt("inlight",value);
    // Serial.print("Prefs.inlight: "); Serial.println(Prefs.getInt("inlight"));

    value = doc["led4"]["time"];
    Prefs.putInt("intime",value);
    // Serial.print("Prefs.intime: "); Serial.println(Prefs.getInt("intime"));
  }
  // Определяемся и обрабатываем команду по интервалам сообщений
  String intrv=doc["intrv"];
  if (intrv != "null")
  {
    value = doc["intrv"]["mode4"];
    Prefs.putInt("mode4",value);
    // Serial.print("Prefs.mode4: "); Serial.println(Prefs.getInt("mode4"));

    value = doc["intrv"]["img"];
    Prefs.putInt("img",value);
    // Serial.print("Prefs.img: "); Serial.println(Prefs.getInt("img"));

    value = doc["intrv"]["tempvl"];
    Prefs.putInt("tempvl",value);
    // Serial.print("Prefs.tempvl: "); Serial.println(Prefs.getInt("tempvl"));

    value = doc["intrv"]["lumin"];
    Prefs.putInt("lumin",value);
    // Serial.print("Prefs.lumin: "); Serial.println(Prefs.getInt("lumin"));

    value = doc["intrv"]["bar"];
    Prefs.putInt("bar",value);
    // Serial.print("Prefs.bar: "); Serial.println(Prefs.getInt("bar"));
  }
  // Закрываем свойства
  Prefs.end();
  
  //String vintrv=doc["vintrv"];
  //Serial.print("vintrv: "); Serial.println(vintrv);
  //Serial.print("vintrv.length(): "); Serial.println(vintrv.length());
}
// ****************************************************************************
// *      Выполнить выборку всех json-сообщений из ответа страницы Lead       *
// ****************************************************************************
void Deser(String httpText)
{
  // Преобразовываем String-ответ в chars-ответ для парсинга
  int str_len = httpText.length()+1; 
  char json[str_len];
  httpText.toCharArray(json, str_len);
  // Создаем объект поиска соответствий
  MatchState ms(json);
  // Формируем регулярное выражение ("соответствие") для поиска json-сообщений:
  // <Lead>{"led4":{"light":25,"time":1996},"intrv":{"mode4":6900,"img":1001,"tempvl":3003,"lumin":2002,"bar":5005}}</Lead>
  const char * match="{[{\"a-z0-9:,%[}%]]*}";
  // Выполняем поиск по соответствию
  int count = ms.GlobalMatch(match,match_callback);
  // Serial.print("count: "); Serial.println(count);
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
        // Изменяем значение счетчика и включаем его в параметр запроса к странице Lead
        iLead=incUINT32T(iLead);
        String queryString = "cycle="+String(iLead);    
        // Готовим структуру для ответа
        tQueryMessage tQuery;                              

        // По умолчанию включаем в параметр запрос изменений
        String sjson="&sjson="+s_COMMON;
        /*
        // Если изменен режим работы вспышки
        if (Led4Chang) 
        {
          queryString = "cycle=-1";    
          s_MODE4 = "{\"led4\":{\"light\":"+String(jlight)+",\"time\":"+String(jtime)+"}}"; 
          sjson="&sjson="+s_MODE4;
        }
        // Если изменены интервалы отправки сообщений
        else if (intrvChang) 
        {
          queryString = "cycle=-2";    
          s_INTRV = "{\"intrv\":{\"mode4\":"+String(jmode4)+",\"img\":"+String(jimg)+",\"tempvl\":"+String(jtempvl)+",\"lumin\":"+String(jlumin)+",\"bar\":"+String(jbar)+"}}"; 
          sjson="&sjson="+s_INTRV;
        }
        */
        queryString=queryString+sjson;
        // Делаем запрос к странице: "https://probatv.ru/Lead40/"
        tQuery = postQuery(urlLead, queryString);
        // Обрабатываем успешный запрос (неуспешный обработан в postQuery)
        if (tQuery.httpCode == HTTP_CODE_OK) Deser(tQuery.httpText);
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

// ************************************************************* kviLead.h ***
