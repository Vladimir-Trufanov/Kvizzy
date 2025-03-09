/** Arduino, Esp32-CAM ******************************************** State.h ***
 * 
 *        Выбрать накопившиеся json-сообщения о состоянии устройств контроллера 
 *            и показаниях датчиков из очереди и отправить их на страницу State 
 * 
 * v3.3.5, 16.01.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
 * String frame="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";
 *
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

  // Зацикливаем задачу
  for (;;)
  {
    //Serial.println("*** vState ***");
    #ifdef tmr_STATE
      // Отправляем на сайт все, накопившиеся, сообщения
      queState.PostAll();
    #endif
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
    //Serial.print("State: ");  // передали префикс
    //Serial.println(mess);     // передали сообщение

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
   if (toTrassState) {Serial.print("To State: "); Serial.println(queryString);}  
   tQuery = postQuery(ehttp, queryString);
   // Обрабатываем успешный запрос 
   if (tQuery.httpCode == HTTP_CODE_OK) 
   {
      // Трассируем ответ от State
      if (fromTrassState) {Serial.print("<= State: "); Serial.println(tQuery.httpText);}
      // Обновляем json-документ
      oJSON.UpdateDoc(String(mess));
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
