/** Arduino, Esp32-CAM ******************************************** State.h ***
 * 
 *        Выбрать накопившиеся json-сообщения о состоянии устройств контроллера 
 *            и показаниях датчиков из очереди и отправить их на страницу State 
 * 
 * v3.3.5, 16.01.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>

// * Задача FreeRTOS **********************************************************
// *   Выбрать накопившиеся json-сообщения о состоянии устройств контроллера  *
// *     и показаниях датчиков из очереди и отправить их на страницу State    *
// ****************************************************************************
void vState(void* pvParameters) 
{
   /*
   // Имитируем зависание микроконтроллера с помощью опознанного числа,
   // принятого в последовательном порту
   if (iCreateSit == loopingLed33) MimicMCUhangEvent("Led33");  
   */ 
   /*
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
   */
   // Зацикливаем задачу
   for (;;)
   {
      // Serial.println("*** vState ***");
      // Отправляем на сайт все, накопившиеся, сообщения
      queState.PostAll();
      // Пропускаем интервал 986 мсек
      vTaskDelay(986/portTICK_PERIOD_MS); 
      // Отмечаем флагом, что цикл задачи успешно завершен   
      fwdtState = true;
   }
}

// ****************************************************************************
// *                    Передать сообщения на страницу State                  *
// *     и показаниях датчиков из очереди и отправить их на страницу State    *
// ****************************************************************************
inline void transState(char *mess, char *prefix) 
{
   // Serial.print("State: ");  // передали префикс
   // Serial.println(mess);     // передали сообщение

   // Подключаемся к веб-странице
   //String shttp="https://doortry.ru/State/e?Com=";  // Ответ: 404
   //String shttp="https://doortryi.ru/State/?Com=";  // Ответ: -1
   //String shttp="http://doortry.ru/State/?Com=";    // Ответ: 301

   // Готовим запрос к странице State
   String ehttp=shttp+"State/";
   // Изменяем значение счетчика и включаем его в параметр запроса к странице State
   iState=incUINT32T(iState);
   String queryString = "cycle="+String(iState);    
   // Готовим структуру для ответа
   tQueryMessage tQuery;                              
   // Инициируем счетчик нечастой трассировки успешных запросов
   int iTrass=0;
   // Включаем в параметр запроса json-сообщение
   String sjson=String(mess); sjson="&sjson="+sjson;
   queryString=queryString+sjson;
   // Трассируем запрос к State
   if (isTrassState) {Serial.print("To State: "); Serial.println(queryString);}  
   tQuery = postQuery(ehttp, queryString);
   // Обрабатываем успешный запрос 
   if (tQuery.httpCode == HTTP_CODE_OK) 
   {
      // Трассируем ответ от State
      if (isTrassState) {Serial.print("<= State: "); Serial.println(tQuery.httpText);}
   }
   else
   // Реагируем на ошибку Post-запроса
   {
      Serial.print("Ошибка Post-запроса: "); Serial.println(tQuery.httpCode);
   }
   // Делаем паузу 10 тиков - дать наверху серверу передохнуть
   vTaskDelay(10/portTICK_PERIOD_MS); 
}
// **************************************************************** State.h ***
