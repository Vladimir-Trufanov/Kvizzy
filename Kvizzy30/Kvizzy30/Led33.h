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
#include "define_kvizzy.h"   // подключили общие определения 

// Определяем состояния светодиода с обратной логикой
#define inHIGH LOW
#define inLOW  HIGH 

// Пины для мигания лампочек
#define LedWorkEsp32Cam  33     // контакт рабочего светодиода

// Определяем переменную прежнего состояния светодиода
volatile int oldStatus=inLOW; 

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
   int LedStatus=digitalRead(LedWorkEsp32Cam);
   // Если состояние светодиода не изменилось, то возврашаем пустой json
   if (LedStatus==oldStatus) return jstr;
   // Если состояние светодиода изменилось готовим новую json-строку
   else
   {
      if (LedStatus==inHIGH) doc["led33"][0]["status"]="inHIGH";
      else doc["led33"][0]["status"]="inLOW";
      serializeJson(doc,jstr);
      oldStatus=LedStatus;
      return jstr;
   }
}

void vLed33(void* pvParameters) 
{
   for (;;)
   {
      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (inumber == 1) MimicMCUhangEvent("Led33");   

      String jstr=getLed33(sjson);
      // Если состояние светодиода изменилось, то есть получена не пустая 
      // json-строка, то передаем его состояние на страницу State 
      if (jstr!=jempty)
      {
         Serial.print("getLed33: ");
         Serial.println(jstr);
      }
      vTaskDelay(507/portTICK_PERIOD_MS);   
      flag[0] = 1;
   }
}

#endif

// **************************************************************** Led33.h ***
