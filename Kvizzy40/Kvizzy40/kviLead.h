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
// *  Проверить, изменились ли параметры действующего режима работы вспышки   *
// ****************************************************************************
bool isChangeLed4(String sjson)
{
  JsonDocument doc;
  deserializeJson(doc, sjson);

  inlight = doc["led4"]["light"];
  if (inlight==jlight)
  {
    Serial.print("inlight: "); Serial.println(inlight);
  }
  else
  {
    Serial.print("inlight: "); Serial.print(inlight); Serial.print(" jlight: "); Serial.println(jlight);
    return true;
  }
  intime = doc["led4"]["time"];
  if (intime==jtime)
  {
    Serial.print("intime: "); Serial.println(intime);
  }
  else
  {
    Serial.print("intime: "); Serial.print(intime); Serial.print(" jtime: "); Serial.println(jtime);
    return true;
  }
  return false;
}
// ****************************************************************************
// *    Проверить, изменились ли интервалы подачи сообщений от контроллера    *
// ****************************************************************************
bool isChangeIntrv(String sjson)
{
  JsonDocument doc;
  deserializeJson(doc, sjson);

  // mode4 - принятый режим работы Led4 
  mode4 = doc["intrv"]["mode4"];
  if (mode4==jmode4)
  {
    Serial.print("mode4: "); Serial.println(mode4);
  }
  else
  {
    Serial.print("mode4: "); Serial.print(mode4); Serial.print(" jmode4: "); Serial.println(jmode4);
    return true;
  }
  // img - принятая подача изображения 
  img = doc["intrv"]["img"];
  if (img==jimg)
  {
    Serial.print("img: "); Serial.println(img);
  }
  else
  {
    Serial.print("img: "); Serial.print(img); Serial.print(" jimg: "); Serial.println(jimg);
    return true;
  }
  // tempvl - принятые температура и влажность 
  tempvl = doc["intrv"]["tempvl"];
  if (tempvl==jtempvl)
  {
    Serial.print("tempvl: "); Serial.println(tempvl);
  }
  else
  {
    Serial.print("tempvl: "); Serial.print(tempvl); Serial.print(" jtempvl: "); Serial.println(jtempvl);
    return true;
  }
  // lumin - принятая освещённость камеры
  lumin = doc["intrv"]["lumin"];
  if (lumin==jlumin)
  {
    Serial.print("lumin: "); Serial.println(lumin);
  }
  else
  {
    Serial.print("lumin: "); Serial.print(lumin); Serial.print(" jlumin: "); Serial.println(jlumin);
    return true;
  }
  // bar - принятое атмосферное давление
  bar = doc["intrv"]["bar"];
  if (bar==jbar)
  {
    Serial.print("bar: "); Serial.println(bar);
  }
  else
  {
    Serial.print("bar: "); Serial.print(bar); Serial.print(" jbar: "); Serial.println(jbar);
    return true;
  }
  return false;
}

// ****************************************************************************
// *    Выбрать и обработать текущее json-сообщение из ответа страницы Lead   *
// ****************************************************************************
void match_callback(const char * match,const unsigned int length,const MatchState & ms)
{
  // Выбираем очередной найденный фрагмент
  // {"led4":{"light":25,"time":1996},"intrv":{"mode4":6900,"img":1001,"tempvl":3003,"lumin":2002,"bar":5005}}
  String sjson = String(match);
  sjson = sjson.substring(0,length);
  Serial.print("sjson: "); Serial.println(sjson); 

  JsonDocument doc;
  deserializeJson(doc, sjson);

  // Определяемся и обрабатываем команду по режиму вспышки
  String led4=doc["led4"];
  if (led4 != "null")
  {
    Led4Start=isChangeLed4(sjson);
  }
  // Определяемся и обрабатываем команду по интервалам сообщений
  String intrv=doc["intrv"];
  if (intrv != "null")
  {
    intrvStart=isChangeIntrv(sjson);
  }

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
  // Трассируем ответ от Lead
  // Serial.print("json: "); Serial.println(json);
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
        // Готовим запрос к странице Lead
        String ehttp=urlHome+"/Lead40/";
        // Изменяем значение счетчика и включаем его в параметр запроса к странице State
        iLead=incUINT32T(iLead);
        String queryString = "cycle="+String(iLead);    
        // Готовим структуру для ответа
        tQueryMessage tQuery;                              

        // По умолчанию включаем в параметр запрос изменений
        String sjson="&sjson="+s_COMMON;
        // Если изменен режим работы вспышки
        if (Led4Start) 
        {
          s_MODE4 = "{\"led4\":{\"light\":"+String(jlight)+",\"time\":"+String(jtime)+"}}"; 
          sjson="&sjson="+s_MODE4;
          Serial.println("Отправляем s_MODE4"); 
        }
        // Если изменены интервалы отправки сообщений
        else if (intrvStart) 
        {
          s_INTRV = "{\"intrv\":{\"mode4\":"+String(jmode4)+",\"img\":"+String(jimg)+",\"tempvl\":"+String(jtempvl)+",\"lumin\":"+String(jlumin)+",\"bar\":"+String(jbar)+"}}"; 
          sjson="&sjson="+s_INTRV;
          Serial.println("Отправляем s_INTRV"); 
        }

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
