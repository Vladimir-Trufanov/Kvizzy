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

// Определяем заголовок для таймера вспышки
hw_timer_t *timerLed4 = NULL;
int nLight=5000;        // время (мсек) свечения в цикле 
int nNoLight=5000;      // время (мсек) НЕ свечения в цикле 
int fLight=shimLOW;     // флаг свечения светодиода
int lastled4=millis();  // текущее время (уходящее в прошлое)
void ARDUINO_ISR_ATTR onTimerLed4() 
{
  // Отрабатываем режим
  if (fLight==shimHIGH)
  {
    Serial.print("Не горело (мс): "); Serial.println(millis() - lastled4);
    analogWrite(LED_PIN_4, shimHIGH);
    fLight=shimLOW;  
    lastled4=millis(); 
    timerAlarm(timerLed4, 200000, true, 0);
  }
  else
  {
    Serial.print("Светилось (мс): "); Serial.println(millis()-lastled4);
    analogWrite(LED_PIN_4, shimLOW);
    fLight=shimHIGH;  
    lastled4=millis(); 
    timerAlarm(timerLed4, 1800000, true, 0);
  }
}


// ****************************************************************************
// *               Передать режим работы вспышки на страницу State            *
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
{
  for (;;)
  {
    Serial.println("*** vLed4 ***");
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
      // Извлекаем постоянные данные
      Prefs.begin("KvizzyPrefs", false);
      Led4Start=Prefs.getBool("Led4Start");
      jlight=Prefs.getInt("jlight");
      jtime=Prefs.getInt("jtime");
      //Serial.println(jlight);
      //Serial.println(jtime);
      Prefs.end();
      // Если поступила команда на включение режима работы контрольного светодиода
      // (или произошла перезагрузка контроллера)
      if (Led4Start)
      {
        // Рассчитываем времена свечения и несвечения контрольного светодиода
        nLight=jtime*jlight/100;  // 2000*10/100=200
        nNoLight=jtime-nLight;    // 2000-200=1800
        // Формируем json-сообщение для отправки на State
        // s4_MODE = "{\"led4\":[{\"regim\":1,\"light\":"+String(jlight)+",\"time\":"+String(jtime)+"}]}";
        // Сбрасываем флаг включения режима
        Prefs.begin("KvizzyPrefs", false);
        Led4Start=false; 
        Prefs.putBool("Led4Start",Led4Start);
        Prefs.end();
      }

      /*
      // Отрабатываем режим
      if (fLight==shimHIGH)
      {
        Serial.print("Не горело (мс): "); Serial.println(millis() - last);
        last=millis();  // текущее время (уходящее в прошлое)
        analogWrite(LED_PIN_4, shimHIGH);
      }
      else
      {
        Serial.print("Светилось (мс): "); Serial.println(millis()-last);
        last=millis();  // текущее время (уходящее в прошлое)
        analogWrite(LED_PIN_4, shimLOW);
      }
      */
      // Освобождаем мьютекс
      xSemaphoreGive(HttpMutex);  
    }
    else 
    {  
      Serial.println("vLed4: HttpMutex не был захвачен!");
    }
    /*
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
    */
    // Передаём запрос
    // Led4State(s4_MODE);
    vTaskDelay(2000/portTICK_PERIOD_MS); 
    // Отмечаем флагом, что цикл задачи успешно завершен   
    fwdtLed4 = true;
  }
}

// ************************************************************** kviLed4.h ***
