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
//#include "State.h"    

#define inHIGH LOW           // светодиод с обратной логикой включён
#define inLOW  HIGH          // светодиод с обратной логикой ВЫКЛ

// Определяем переменную прежнего состояния светодиода
//volatile int oLed33Status=inLOW; 
// Определяем время последнего учтенного (отправленного) состояния светодиода
//volatile int mitсLed33=millis();

// ****************************************************************************
// *  Обработать прерывание для контрольного светодиода  при смене состояния  *
// *     33 пина (CHANGE)  и передать в очередь новое состояние контакта      *
// ****************************************************************************

// json-сообщение о включенном состоянии контрольного светодиода
const String s33_HIGH="{\"led33\":[{\"status\":\"inHIGH\"}]}";
// json-сообщение о выключенном контрольном светодиоде
const String s33_LOW="{\"led33\":[{\"status\":\"inLOW\"}]}";

void IRAM_ATTR toggleLedWork()
{
   String inMess;
   // Выбираем состояние устройства
   int Led33Status=digitalRead(PinLedWork);
   if (Led33Status==inHIGH) inMess=queState.SendISR(s33_HIGH);
   else inMess=queState.SendISR(s33_LOW); 
   // Если невозможно отправить сообщение, то сообщаем
   if (inMess!=tisOk) Serial.println(inMess);
}
// * Задача FreRTOS ***********************************************************
// *                Обеспечить определение состояния контрольного светодиода  *
// * ESP32-CAM ("горит - не горит") и передачу данных на страницу сайта State *
// ****************************************************************************
void vLed33(void* pvParameters)
/*
{
  "led33": [
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
   int iTime;         // длительность цикла "горит - не горит" (мсек)
   int iLight;        // процент времени свечения в цикле 
   int nLight;        // время (мсек) свечения в цикле 
   int nNoLight;      // время (мсек) НЕ свечения в цикле 
   int fLight=inLOW;  // флаг свечения светодиода

   for (;;)
   {
      /*
      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (iCreateSit == loopingLed33) MimicMCUhangEvent("Led33");   
      */
      // Если поступила команда на включение режима работы контрольного светодиода
      // (или произошла перезагрузка контроллера)
      if (Led33Start)
      {
         sjson=oJSON.jsongetLed33();
         //Serial.print("sjson:  "); Serial.println(sjson); 
         JsonDocument docL33;
         deserializeJson(docL33, sjson);
         iTime  = docL33["led33"][0]["time"];
         iLight = docL33["led33"][0]["light"];
         // Рассчитываем времена свечения и несвечения контрольного светодиода
         nLight=iTime*iLight/100;
         nNoLight=iTime-nLight;
         // Устанавливаем флаг свечения светодиода
         fLight=inHIGH;
         //Serial.print("nLight:   "); Serial.println(nLight); 
         //Serial.print("nNoLight: "); Serial.println(nNoLight);
         // Сбрасываем флаг включения режима
         Led33Start=false; 
      }
      // Отрабатываем режим
      if (fLight==inHIGH)
      {
         digitalWrite(PinLedWork, inHIGH);
         vTaskDelay(nLight/portTICK_PERIOD_MS); 
         fLight=inLOW;    
      }
      else
      {
         digitalWrite(PinLedWork, inLOW);
         vTaskDelay(nNoLight/portTICK_PERIOD_MS); 
         fLight=inHIGH;    
      }
      // Отмечаем флагом, что цикл задачи успешно завершен   
      fwdtLed33 = true;
   }
}

#endif

// **************************************************************** Led33.h ***
