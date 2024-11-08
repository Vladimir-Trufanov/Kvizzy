/** Arduino, Esp32-CAM ******************************************** Led33.h ***
 * 
 *           Обеспечить определение состояния контрольного светодиода ESP32-CAM
 *               ("горит - не горит") и передачу данных на страницу сайта State
 * 
 * v1.2, 08.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#ifndef led_33
#define led_33
#pragma once            

#include <Arduino.h>
#include "define_kvizzy.h"    

#define inHIGH LOW           // светодиод с обратной логикой включён
#define inLOW  HIGH          // светодиод с обратной логикой ВЫКЛ
#define LedWorkEsp32Cam  33  // пин рабочего светодиода
#define tictrlLed33 15000    // интервал контрольной отправки состояния светодиода

// Определяем переменную прежнего состояния светодиода
volatile int oLed33Status=inLOW; 
// Определяем время последнего учтенного (отправленного) состояния светодиода
volatile int mitсLed33=millis();

// ****************************************************************************
// *          Проверить состояние светодиода и сформировать json-строку       *
// ****************************************************************************
String getLed33(String sjson) 
{
   // Инициируем возвращаемую json-строку   
   String jstr = jempty;
   // Формируем json по состоянию контрольного светодиода 
   JsonDocument filter;
   filter["tidctrl"] = true;            // идентификатор типа контроллера
   filter["nicctrl"] = true; 
   filter["led33"][0]["tiddev"]= true;  // идентификатор типа устройства
   filter["led33"][0]["nicdev"]= true;  // nic устройства
   filter["led33"][0]["status"]= true;  // текущее состояние светодиода     
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   // Выбираем состояние устройства
   int Led33Status=digitalRead(LedWorkEsp32Cam);
   // Если состояние светодиода не изменилось, то, как правило, возврашаем пустой json
   // для того, чтобы не передавать лишний раз данные на серверную страницу.
   // Но если неизменное состояние длится более контрольного времени,
   // то, всё-таки делаем контрольную отправку
   if (Led33Status==oLed33Status) 
   {
      int mitсLed33i=millis();
      if ((mitсLed33i-mitсLed33)>tictrlLed33)
      {
         if (oLed33Status==inHIGH) doc["led33"][0]["status"]="inHIGH";
         else doc["led33"][0]["status"]="inLOW";
         serializeJson(doc,jstr);
         mitсLed33 = mitсLed33i;
      }
      return jstr;
   }
   // Если состояние светодиода изменилось готовим новую json-строку
   else
   {
      if (Led33Status==inHIGH) doc["led33"][0]["status"]="inHIGH";
      else doc["led33"][0]["status"]="inLOW";
      serializeJson(doc,jstr);
      oLed33Status=Led33Status;
      return jstr;
   }
}
// * Задача FreRTOS ***********************************************************
// *                Обеспечить определение состояния контрольного светодиода  *
// * ESP32-CAM ("горит - не горит") и передачу данных на страницу сайта State *
// ****************************************************************************
void vLed33(void* pvParameters) 
{
   for (;;)
   {
      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (iCreateSit == loopingLed33) MimicMCUhangEvent("Led33");   

      String jstr=getLed33(sjson);
      // Если состояние светодиода изменилось, то есть получена не пустая 
      // json-строка, то передаем его состояние на страницу State 
      if (jstr!=jempty)
      {
         Serial.print("getLed33: ");
         Serial.println(jstr);
      }
      vTaskDelay(507/portTICK_PERIOD_MS); 
      // Отмечаем флагом, что цикл задачи успешно завершен   
      fwdtLed33 = true;
   }
}

#endif

// **************************************************************** Led33.h ***
