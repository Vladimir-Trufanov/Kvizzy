/** Arduino, Esp32-CAM ************************************ common_kvizzy.h ***
 * 
 *                          Выбрать из очереди и вывести сообщения на периферию
 * 
 * v4.0.0, 28.03.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>

// * Задача FreRTOS ***********************************************************
// *               Выбрать из очереди и вывести сообщения на периферию        *
// ****************************************************************************
void vPrint(void* pvParameters) 
{
  for (;;)
  {
    // Serial.println("*** vPrint ***");

    /*
    Prefs.begin("KvizzyPrefs", false);
      //Led4Start=Prefs.getBool("Led4Start");
    jlight=Prefs.getInt("jlight");
    jlight++;
    Prefs.putInt("jlight",jlight);
    Prefs.end();
    */

    // Сбрасываем все сообщения на периферию через transPrint
    queMessa.PostAll();
    // Отмечаем флагом, что цикл задачи успешно завершен   
    fwdtPrint = true;
    vTaskDelay(983/portTICK_PERIOD_MS); 
 }
}
// ****************************************************************************
// *                        Передать сообщение на периферию                   *
// ****************************************************************************
inline void transPrint(char *mess, char *prefix="") 
{
  Serial.println(mess);  // передали сообщение
}

// ******************************************************** common_kvizzy.h ***
