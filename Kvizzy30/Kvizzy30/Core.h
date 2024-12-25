/** Arduino, Esp32-CAM ********************************************* Core.h ***
 * 
 *                                 Обеспечить определение состояния процессоров
 *                                    и передачу данных на страницу сайта State
 * 
 * v1.3, 25.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#ifndef h_Core
#define h_Core
#pragma once            

#include <Arduino.h>
#include "define_kvizzy.h"    
#include "State.h"    

// ****************************************************************************
// *          Проверить состояние процессора и сформировать json-строку       *
// ****************************************************************************
String getCoreX(String sjson, String imjaCore="core1") 
{
   JsonDocument filter;
   filter["idctrl"] = true; 
   filter[imjaCore][0]["iddev"]= true;
   filter[imjaCore][0]["minstack"]= true;
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   doc[imjaCore][0]["minstack"]=34;
   String str = "";
   serializeJson(doc,str);
   return str;
}
/*
// * Задача FreRTOS ***********************************************************
// *                            Обеспечить определение состояния процессора 0 *
// *                                и передачу данных на страницу сайта State *
// ****************************************************************************
void vCore0(void* pvParameters) 
{
   for ( ;; )
   {
      String str=getCoreX(sjson,"Core0");
      //Serial.print("getCore0: ");
      //Serial.println(str);

      vTaskDelay(5000/portTICK_PERIOD_MS);   // 5 секунд задача 
      fwdtCore0 = true;
      if (iCreateSit == loopingCore0) MimicMCUhangEvent("Core0");   
   }
}
*/
// * Задача FreRTOS ***********************************************************
// *                            Обеспечить определение состояния процессора 1 *
// *                                и передачу данных на страницу сайта State *
// ****************************************************************************
void vCore1(void* pvParameters) 
{
   for ( ;; )
   {
      String str=getCoreX(sjson);
      //Serial.print("getCore1: ");
      //Serial.println(str);
      vTaskDelay(6000/portTICK_PERIOD_MS);   // 6 секунд задача
      fwdtCore1 = true;
      if (iCreateSit == 3) MimicMCUhangEvent("Core1");   
   }
}

#endif

// ***************************************************************** Core.h ***
