/** Arduino, Esp32-CAM *************************************** Kvizzy40.ino ***
 * 
 *         Kvizzy40 - исполнительный приложение умного хозяйства на контроллере 
 *          ESP32-CAM для управления светодиодом, снятия показаний температуры,
 *             влажности и формирования потока изображений наблюдаемого объекта
 * 
 * v4.0.0, 28.03.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 31.05.2024
 * 
**/

#include <Arduino.h>
#include <WiFi.h>

#include "define_kvizzy.h"   // общие определения 
//#include "common_kvizzy.h"   // общие функции  


// Подключаем задачи
#include "kviStream.h"       // 8-2971  фотографирование и отправка изображения


// Определяем заголовок для сторожевого таймера
hw_timer_t *timer = NULL;
// Инициируем спинлок критической секции в обработчике таймерного прерывания
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
// Обрабатываем прерывание сторожевого таймера
void IRAM_ATTR onTimer() 
{
  portENTER_CRITICAL_ISR(&timerMux);
  // Если флаги всех задач установлены в 1, 
  // то сбрасываем флаги задач и счетчик сторожевого таймера
  if (fwdtLoop==true 
    /* 
    && fwdtLed33==true &&  
    fwdtLead==true && fwdtState==true && fwdtPrint==true
    */
  && fwdtStream==true) 
  {
    // Сбрасываем флаги задач
    fwdtLoop  = false;
      /*
      fwdtLed33 = false;
      fwdtLead = false;
      fwdtState = false;
      fwdtPrint = false;
      */
    fwdtStream = false;
    // "Пинаем собаку" - сбрасываем счетчик сторожевого таймера
    timerWrite(timer, 0);
  }
  // Иначе перезагружаем контроллер
  else 
  {
    ESP.restart();
  }
  portEXIT_CRITICAL_ISR(&timerMux);
}


/*
#include <HTTPClient.h>

#include "AttachSNTP.h"
#include "jsonBase.h"       

// Определяем директивы отладки
// #define tmr_TRACEMEMORY

// Управляем включением задач
// #define tmr_LEAD
#define tmr_STATE   // 2025-03-08, state не отключаем, пусть сообщения обрабатываются
#define tmr_STREAM
// Определяем объект для синхронизации времени 
TAttachSNTP oSNTP;
// Определяем объект для работs с документом JSON
TJsonBase oJSON;

// Подключаем файлы обеспечения передачи и приёма сообщений через очередь                
#include "Kvizzy30_Message.h" // сообщения приложения  
#include "QueMessage.h"       // заголовочный файл класса TQueMessage                    
#include "QueChar.h"          // заголовочный файл класса TQueChar                        
// Назначаем объекты работы с сообщениями через очередь                                   
TQueMessage queMessa(amessAPP,SizeMess,tmk_APP);    // для периферии                                     
TQue queState;                                      // для страницы State

#include "define_kvizzy.h"   // общие определения 
#include "common_kvizzy.h"   // общие функции  

// Подключаем задачи и деятельности
#include "Lead.h"            //  9-897 запрос контроллера на изменение состояний устройств
#include "State.h"           //  8-986 выборка сообщений о состоянии и отправка 
#include "tStream.h"         // 10-42  фотографирование и отправка изображения
#include "Led33.h"           // обработка контрольного светодиода 
// Обработка прерывания для вспышки при изменении состояние 4 контакта с LOW на HIGH (RISING).
void IRAM_ATTR onLedFlash()
{
}
*/

// ****************************************************************************
// *                         Начальная настройка                              *
// ****************************************************************************
void setup() 
{
  // Включаем последовательный порт
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("Последовательный порт работает!");
  // Подключаемся к Wi-Fi сети
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  Serial.print("Соединяемся с Wi-Fi .");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("К Wi-Fi сети подключились!");
  
  // Подключаем задачу по фотографированию и отправке Base24-изображения на страницу Stream
  xTaskCreatePinnedToCore(
    vStream,                // Task function
    "Stream",               // Task name
    3072,                   // Stack size
    NULL,                   // Parameters passed to the task function
    8,                      // Priority
    NULL,                   // Task handle
    1);

 
  /*
  // Создаём объект и строку всего JSON-документа         
   oJSON.Create();
   Serial.println("");
  */
  
  /*
   // Проверяем системное время, если время еще не установлено, производим его 
   // синхронизацию по протоколу SNTP с серверами точного времени,
   oSNTP.Create();
   Serial.println("");
   
   // Создаем очередь сообщений на периферию                                                                   
   String inMess=queMessa.Create();                                                      
   // Если не получилось, сообщаем "Очередь не была создана и не может использоваться"    
   if (inMess==QueueNotCreate) Serial.println(QueueNotCreate);                           
   // Если очередь получилась, то отмечаем  "Очередь сформирована"                       
   else {Serial.print(QueueBeformed); Serial.println(" для сообщений на периферию");}                                                   
   // Подключаем функцию передачи сообщения на периферию                                 
   queMessa.attachFunction(transPrint);  

   // Создаем очередь сообщений на страницу State                                                                    
   inMess=queState.Create();                                                      
   // Если не получилось, сообщаем "Очередь не была создана и не может использоваться"    
   if (inMess==tQueueNotCreate) Serial.println(tQueueNotCreate);                         
   // Если очередь получилась, то отмечаем  "Очередь сформирована"                      
   else  {Serial.print(QueueBeformed); Serial.println(" для сообщений на страницу State");}         
   // Подключаем функцию передачи сообщения на страницу State                                
   queState.attachFunction(transState);                                                  
   // Отмечаем, что соединение с Wi-Fi установлено
   inMess=queMessa.Send(tmt_NOTICE,WifiEstablished,tmk_Queue);
   if (inMess!=isOk) Serial.println(inMess); 

   Serial.println("");

   // Переводим контакты лампочек в режим вывода и подключаем обработку прерываний
   pinMode(PinLedWork,OUTPUT);    // контрольный светодиод
   attachInterrupt(PinLedWork,toggleLedWork,CHANGE);
   pinMode(PinLedFlash,OUTPUT);   // вспышка
   attachInterrupt(PinLedFlash,onLedFlash,RISING);

   // Подключаем задачу определения состояния контрольного светодиода ESP32-CAM 
   // ("горит - не горит") и передачу данных на страницу сайта State  
   xTaskCreatePinnedToCore(
      vLed33,                 // Task function
      "Led33",                // Task name
      4096,                   // Stack size
      NULL,                   // Parameters passed to the task function
      5,                      // Priority
      NULL,                   // Task handle
      1); 
   // Выполнить регулярный (по таймеру) запрос контроллера на изменение   
   // состояний его устройств к странице Lead             
   xTaskCreatePinnedToCore(
      vLead,                  // Task function
      "Lead",                 // Task name
      2048,                   // Stack size
      NULL,                   // Parameters passed to the task function
      8,                      // Priority
      NULL,                   // Task handle
      1);
   // Выбрать накопившиеся json-сообщения о состоянии устройств контроллера 
   // и показаниях датчиков из очереди и отправить их на страницу State 
   xTaskCreatePinnedToCore(
      vState,                 // Task function
      "State",                // Task name
      3072,                   // Stack size
      NULL,                   // Parameters passed to the task function
      8,                      // Priority
      NULL,                   // Task handle
      1);
  */
  /*
   // Выбрать из очереди и вывести сообщения в последовательный порт
   xTaskCreatePinnedToCore(
      vPrint,                 // Task function
      "Print",                // Task name
      2048,                   // Stack size
      NULL,                   // Parameters passed to the task function
      6,                      // Priority
      NULL,                   // Task handle
      1);
   // Создаём объект таймера, устанавливаем его частоту отсчёта (1Mhz)
   timer = timerBegin(1000000);
   // Подключаем функцию обработчика прерывания от таймера - onTimer
   timerAttachInterrupt(timer, &onTimer);
   // Настраиваем таймер: интервал перезапуска - 20 секунд (20000000 микросекунд),
   // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
   // раз (четвертый параметр = 0) 
   timerAlarm(timer, 20000000, true, 0);
*/
}

// Инициируем прием кодов и заполнение строки
//int incomingByte = 0; 
//int incomingCode = 0; 
//String data = "";

// ****************************************************************************
// *                           Базовый цикл приложения                        *
// ****************************************************************************
void loop() 
{
  Serial.println("*** loop ***");
  /*
   // Проверяем, есть ли байты в последовательном порту
   if (Serial.available() > 0) 
   {
      // Читаем очередной байт
      incomingByte = Serial.read();
      // Получаем по коду символ
      char character = (char)incomingByte;
      // Выводим код полученного символа
      //Serial.print("Получен: ");
      //Serial.println(incomingByte);
      // Если не "перевод строки" то складываем
      // символ в формируемую строку
      if (character != '\n')
      {
         data.concat(character);
      }
      // Иначе выводим текст
      else
      {
         // Выводим текст в последовательный порт
         incomingCode = data.toInt();
         Serial.print("["); Serial.print(incomingCode); Serial.print("] "); 
         
         // Управляем трассировкой сообщений к State 
         if (incomingCode==enabTrassState) 
         {
            Serial.println("Включение трассировки сообщений к State!"); 
            toTrassState=true;
            fromTrassState=true;
         }
         else if (incomingCode==disaTrassState) 
         {
            Serial.println("Отключение трассировки сообщений к State!"); 
            toTrassState=false;
            fromTrassState=false;
         }
         // Готовим прием нового текста
         data = "";
      }
   }
  */
  // Отмечаем флагом, что цикл задачи успешно завершен   
  fwdtLoop = true;
  // Ничего не делаем пол секунды
  vTaskDelay(521/portTICK_PERIOD_MS); 
}

// *********************************************************** Kvizzy40.ino ***
