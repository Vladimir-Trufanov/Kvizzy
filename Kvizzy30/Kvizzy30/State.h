/** Arduino, Esp32-CAM ******************************************** State.h ***
 * 
 *        Выбрать накопившиеся json-сообщения о состоянии устройств контроллера 
 *            и показаниях датчиков из очереди и отправить их на страницу State 
 * 
 * v3.3.3, 27.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>


 // * Задача FreRTOS ***********************************************************
// *   Выбрать накопившиеся json-сообщения о состоянии устройств контроллера  *
// *     и показаниях датчиков из очереди и отправить их на страницу State    *
// ****************************************************************************
void vState(void* pvParameters) 
{
   // Подключаемся к веб-странице
   //String shttp="https://doortry.ru/State/e?Com=";  // Ответ: 404
   //String shttp="https://doortryi.ru/State/?Com=";  // Ответ: -1
   //String shttp="http://doortry.ru/State/?Com=";    // Ответ: 301

   // Готовим запрос к странице State
   String ehttp=shttp+"State/";                   // запрос
   String queryString = "cycle="+String(iState);  // параметры
   tQueryMessage tQuery;                          // ответ
   // Инициируем счетчик нечастой трассировки успешных запросов
   int iTrass=0;
   // Зацикливаем задачу
   for (;;)
   {
      /*
      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (iCreateSit == loopingLed33) MimicMCUhangEvent("Led33");  
      */ 

      String jstr="&cjson=";
      String sjson="95";
      jstr +=sjson;

      iState++;
      // Делаем запрос к State
      tQuery = postQuery(ehttp, queryString);
      // Обрабатываем успешный запрос 
      if (tQuery.httpCode == HTTP_CODE_OK) 
      {
         // Трассировочное сообщение в очередь
         iTrass++;
         if (iTrass>7)
         {
            iTrass=0;
            Serial.print(iState); Serial.print("-State: "); Serial.println(tQuery.httpText);
         }
      }
      // Реагируем на ошибку Post-запроса
      {
         // Пока ничего не делаем, сообщения об ошибках отправлены в postQuery   
      }

      // Если разрешено, трассируем память контроллера
      #ifdef tmr_TRACEMEMORY
         // Получаем полный размер кучи в памяти
         printf("Общий размер ВСТРОЕННОЙ памяти:     %u\n", ESP.getHeapSize());
         // Количество доступной кучи в памяти
         printf("Оставшаяся доступная память в куче: %u\n", ESP.getFreeHeap());
         // Самый низкий уровень свободной кучи с момента загрузки
         printf("Минимальная свободная с загрузки:   %u\n", ESP.getMinFreeHeap());
         // Размер общей кучи SPI PSRAM
         printf("Общий размер SPI PSRAM:             %u\n", ESP.getPsramSize());
         // Количество свободной PSRAM
         printf("Количество свободной PSRAM:         %d\n", ESP.getFreePsram());
         // Минимальный размер свободной памяти в SPI RAM
         printf("Минимум свободной SPI PSRAM:        %d\n", ESP.getMinFreePsram());
         // Размер самого большого блока PSRAM, который может быть выделен
         printf("Самый большой блок для выделения:   %d\n", ESP.getMaxAllocPsram());
      #endif
      
      // Отмечаем флагом, что цикл задачи успешно завершен   
      fwdtState = true;
      // Пропускаем интервал 986 мсек
      vTaskDelay(986/portTICK_PERIOD_MS); 
   }
}

// **************************************************************** State.h ***
