/** Arduino, Esp32-CAM ****************************************** kviLed4.h ***
 * 
 *    Обеспечить управление 4 сведодиодом ESP32-CAM в режиме "горит - не горит" 
 *                                    и передачу данных на страницу сайта State
 *                                    
 * https://docs.espressif.com/projects/arduino-esp32/en/latest/api/ledc.html
 * 
 * v4.4.2, 27.05.2025                                 Автор:      Труфанов В.Е.
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
  // Если время горения определено, как менее 10 мсек, то выключаем вспышку на весь период
  if (nLight<=10000)
  {
    //Serial.print("Светилось (мс): "); Serial.println(millis()-lastled4);
    analogWrite(LED_PIN_4, shimLOW);
    fLight=shimHIGH;  
    lastled4=millis(); 
    timerAlarm(timerLed4, nNoLight+nLight, true, 0);
  } 
  // Если время горения определено, как менее 10 мсек, то ВКЛЮЧАЕМ ВСПЫШКУ вспышку на весь период
  else if (nNoLight<=10000)
  {
    //Serial.print("Не горело (мс): "); Serial.println(millis() - lastled4);
    analogWrite(LED_PIN_4, shimHIGH);
    fLight=shimLOW;  
    lastled4=millis(); 
    timerAlarm(timerLed4, nNoLight+nLight, true, 0);
  }
  else if (fLight==shimHIGH)
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
  //Serial.print("nLight: "); Serial.println(nLight);
}
// ****************************************************************************
// *               Передать режим работы вспышки на страницу State            *
// ****************************************************************************
void Led4State(String inJson) 
{
  // Изменяем значение счетчика и включаем его в параметр запроса к странице State
  iState=incUINT32T(iState);
  String queryString = "cycle="+String(iState);    
  // Готовим структуру для ответа
  tQueryMessage tQuery;                              
  // Включаем в параметр запроса номер управляющей json-команды 
  // num ::= номер управляющей json-команды (-1 -> s4_MODE,-2 -> s_INTRV)
  String numm="&num=-1";
  queryString=queryString+numm;
  // Включаем в параметр запроса json-сообщение
  String sjson="&sjson="+inJson;
  queryString=queryString+sjson;
  // Делаем запрос к странице: "https://probatv.ru/State40/"
  tQuery = postQuery(urlState, queryString);
}
// * Задача FreRTOS ***********************************************************
// *          Обеспечить управление 4 сведодиодом ESP32-CAM в режиме "горит - *
// *                      не горит" и передачу данных на страницу сайта State *
// ****************************************************************************
void vLed4(void* pvParameters)
{
  int last = millis();     // текущее время (уходящее в прошлое)
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
      // Выбираем для проверки прежние и новые данные режима работы вспышки
      // (изменились ли параметры режима работы)
      Prefs.begin("KvizzyPrefs", false);
      jlight=Prefs.getInt("jlight",10);
      inlight=Prefs.getInt("inlight",10);
      jtime=Prefs.getInt("jtime",2000);
      intime=Prefs.getInt("intime",2000);
      Prefs.end();
      // Проверяем, поступила ли команда на изменение режима работы вспышки
      // (не изменились ли параметры режима работы)
      bool Led4Start=false;    // true - изменён режим работы вспышки 
      if (jtime!=intime) Led4Start=true;
      if (jlight!=inlight) Led4Start=true;
      // Если команда поступила
      if (Led4Start)
      {
        // Устанавливаем новые значения параметров
        Prefs.begin("KvizzyPrefs", false);
        Prefs.putInt("jlight",inlight);
        jlight=Prefs.getInt("jlight",10);
        Prefs.putInt("jtime",intime);
        jtime=Prefs.getInt("jtime",2000);
        Prefs.end();
        // Рассчитываем времена свечения и несвечения вспышки
        nLight=jtime*jlight/100;  // 2000*10/100=200
        nNoLight=jtime-nLight;    // 2000-200=1800
        nLight=nLight*1000;       // 200000 мкс
        nNoLight=nNoLight*1000;   // 1800000 мкс
        // Формируем json-сообщение для отправки на State
        s_MODE4 = "{\"led4\":{\"light\":"+String(jlight)+",\"time\":"+String(jtime)+"}}"; 
        // Передаём запрос с режимом работы вспышки на страницу State    
        Led4State(s_MODE4);
        // Начинаем отсчет времени для передачи сообщения при неизменном состоянии
        last = millis();     // текущее время (уходящее в прошлое)
      }
      // Если истекло контрольное время неизменного состояния режима работы вспышки
      if ((millis()-last)>jmode4)
      {
        // Формируем json-сообщение для отправки на State
        s_MODE4 = "{\"led4\":{\"light\":"+String(jlight)+",\"time\":"+String(jtime)+"}}"; 
        // Передаём запрос с режимом работы вспышки на страницу State    
        Led4State(s_MODE4);
        // Начинаем отсчет времени для передачи сообщения при неизменном состоянии
        last = millis();     // текущее время (уходящее в прошлое)
      }
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
