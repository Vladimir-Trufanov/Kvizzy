/** Arduino, Esp32-CAM ****************************************** kviLed4.h ***
 * 
 *    Обеспечить управление 4 сведодиодом ESP32-CAM в режиме "горит - не горит" 
 *                                    и передачу данных на страницу сайта State
 *                                    
 * https://docs.espressif.com/projects/arduino-esp32/en/latest/api/ledc.html
 * 
 * v2.1.1, 28.04.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            

#include <Arduino.h>
#include "define_kvizzy.h"   

#define shimHIGH   1         // яркость включенной вспышки
#define shimLOW    0         // яркость выключенной вспышки

hw_timer_t *timerLed4=NULL;  // заголовок для таймера вспышки
int fLight=shimLOW;          // флаг свечения светодиода
int lastled4=millis();       // текущее время (уходящее в прошлое)

// ****************************************************************************
// *   Включать/выключать светодиод вспышки по прерываниям в заданном режиме  *
// ****************************************************************************
void ARDUINO_ISR_ATTR onTimerLed4() 
{
  if (fLight==shimHIGH)
  {
    //Serial.print("Не горело (мс): "); Serial.println(millis() - lastled4);
    analogWrite(LED_PIN_4, shimHIGH);
    fLight=shimLOW;  
    lastled4=millis(); 
    timerAlarm(timerLed4, nLight, true, 0);
  }
  else
  {
    //Serial.print("Светилось (мс): "); Serial.println(millis()-lastled4);
    analogWrite(LED_PIN_4, shimLOW);
    fLight=shimHIGH;  
    lastled4=millis(); 
    timerAlarm(timerLed4, nNoLight, true, 0);
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
// * Задача FreRTOS ***********************************************************
// *     Отработать заданный режим работы контрольного светодиода ESP32-CAM   *
// *               ("горит - не горит") или отключить его работу              *
// ****************************************************************************
void vLed4(void* pvParameters)
{
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
      if (iCreateSit == loopingLed4) MimicMCUhangEvent("Led4");   
      */
      // Если поступила команда на включение режима работы контрольного светодиода
      // (или произошла перезагрузка контроллера)
      if (Led4Start)
      {
        // Выбираем и пересчитываем параметры действующего режима работы вспышки
        iniPMem(Prefs); 
        // Сбрасываем флаг включения режима
        Prefs.begin("KvizzyPrefs", false);
        Led4Start=false; 
        Prefs.putBool("Led4Start",Led4Start);
        Prefs.end();
      }
      // Передаём запрос
      Led4State(s4_MODE);
      // Освобождаем мьютекс
      xSemaphoreGive(HttpMutex);  
    }
    else 
    {  
      Serial.println("vLed4: HttpMutex не был захвачен!");
    }

    //Serial.println(jlight);
    //Serial.println(jtime);
    //Serial.println(nLight);
    //Serial.println(nNoLight);

    vTaskDelay(2000/portTICK_PERIOD_MS); 
    // Отмечаем флагом, что цикл задачи успешно завершен   
    fwdtLed4 = true;
  }
}

// ************************************************************** kviLed4.h ***
