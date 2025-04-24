/** Arduino, Esp32-CAM ****************************************** kviLed4.h ***
 * 
 *    Обеспечить управление 4 сведодиодом ESP32-CAM в режиме "горит - не горит" 
 *                                    и передачу данных на страницу сайта State
 *                                    
 * https://docs.espressif.com/projects/arduino-esp32/en/latest/api/ledc.html
 * 
 * v2.1.0, 24.04.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            

#include <Arduino.h>
#include "define_kvizzy.h"   

#define shimHIGH 1  // яркость включенной вспышки
#define shimLOW  0  // яркость выключенной вспышки

// ****************************************************************************
// *                    Передать сообщения на страницу State                  *
// *     и показаниях датчиков из очереди и отправить их на страницу State    *
// ****************************************************************************
void Led4State(String inJson) 
{
  
  // Готовим запрос к странице State
  String ehttp=urlHome+"/State40/";
  // Изменяем значение счетчика и включаем его в параметр запроса к странице State
  iState=incUINT32T(iState);
  String queryString = "cycle="+String(iState);    
  // Готовим структуру для ответа
  tQueryMessage tQuery;                              
  // Инициируем счетчик нечастой трассировки успешных запросов
  int iTrass=0;
  // Включаем в параметр запроса json-сообщение
  String sjson="&sjson="+inJson;
  queryString=queryString+sjson;
  // Трассируем запрос к State
  if (toTrassState) {Serial.print("To State: "); Serial.println(queryString);}  
  tQuery = postQuery(ehttp, queryString);
  // Обрабатываем успешный запрос 
  if (tQuery.httpCode == HTTP_CODE_OK) 
  {
    // Трассируем ответ от State
    if (fromTrassState) {Serial.print("<= State: "); Serial.println(tQuery.httpText);}
    // Обновляем json-документ
    // oJSON.UpdateDoc(String(mess));
  }
  else
  // Реагируем на ошибку Post-запроса
  {
    Serial.print("Ошибка Post-запроса: "); Serial.println(tQuery.httpCode);
  }
  
}

// Определяем переменную прежнего состояния светодиода
//volatile int oLed33Status=inLOW; 
// Определяем время последнего учтенного (отправленного) состояния светодиода
//volatile int mitсLed33=millis();

// * Задача FreRTOS ***********************************************************
// *     Отработать заданный режим работы контрольного светодиода ESP32-CAM   *
// *               ("горит - не горит") или отключить его работу              *
// ****************************************************************************
void vLed4(void* pvParameters)
/*
{
  "led4": [
    {
      "nicdev": "led33",
      "tiddev": 1,
      "light": 55,
      "time": 2004,
      "regim": 0,
      "status": "inLOW"
    }
  ]
}
*/
{

  /*
  int iTime;           // длительность цикла "горит - не горит" (мсек)
  int iLight;          // процент времени свечения в цикле 
  */

  int nLight;          // время (мсек) свечения в цикле 
  int nNoLight;        // время (мсек) НЕ свечения в цикле 
  int fLight=shimLOW;  // флаг свечения светодиода
  String jMess;        // отправляемое json-сообщение


  for (;;)
  {
    // Serial.println("*** vLed4 ***");

    // В vLed4 конкурируем за Http-запросы. Будем до 3 секунд ждать освобождения запроса.
    // По опыту от 24.04.2025 на запрос к probatv.ru может уйти до полутора секунд.
    // Если захватить блокировку не получается в течение этого периода времени, прекращаем попытки 
    // и вместо отработки задачи выводим сообщение. 
    if (xSemaphoreTake (HttpMutex, (3000 * portTICK_PERIOD_MS))) 
    { 
      /*
      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (iCreateSit == loopingLed33) MimicMCUhangEvent("Led33");   
      */
      /*
      // Если поступила команда на включение режима работы контрольного светодиода
      // (или произошла перезагрузка контроллера)
      if (Led33Start)
      {
         sjson=oJSON.jsongetLed33();
         // Serial.print("sjson:  "); Serial.println(sjson); 
         JsonDocument docL33;
         deserializeJson(docL33, sjson);
         iTime  = docL33["led33"][0]["time"];
         iLight = docL33["led33"][0]["light"];
         // Рассчитываем времена свечения и несвечения контрольного светодиода
         nLight=iTime*iLight/100;  // 2000*10/100=200
         nNoLight=iTime-nLight;    // 2000-200=1800
         // Устанавливаем флаг свечения светодиода
         fLight=inHIGH;
         Serial.print("nLight:   "); Serial.println(nLight); 
         Serial.print("nNoLight: "); Serial.println(nNoLight);
         // Сбрасываем флаг включения режима
         Led33Start=false; 
      }
      */

      // Рассчитываем времена свечения и несвечения контрольного светодиода в миллисекундах
      nLight=200;
      nNoLight=1800;

      // Отрабатываем режим
      if (fLight==shimHIGH)
      {
        analogWrite(LED_PIN_4, shimHIGH);
        Led4State(s4_HIGH);
      }
      else
      {
        analogWrite(LED_PIN_4, shimLOW);
        Led4State(s4_LOW);
      }
      // Освобождаем мьютекс
      xSemaphoreGive(HttpMutex);  
    }
    else 
    {  
      Serial.println("vLed4: HttpMutex не был захвачен!");
    }
    // Отрабатываем задержку
    if (fLight==shimHIGH)
    {
      vTaskDelay(nLight/portTICK_PERIOD_MS); 
      fLight=shimLOW;    
    }
    else
    {
     vTaskDelay(nNoLight/portTICK_PERIOD_MS); 
      fLight=shimHIGH;    
    }
    // Отмечаем флагом, что цикл задачи успешно завершен   
    fwdtLed4 = true;
  }
}

// Передаём запрос: https://probatv.ru/State40/cycle=31&sjson={"led4":[{"status":"shimHIGH"}]}
// Запрос успешно отправлен: 

// ************************************************************** kviLed4.h ***
