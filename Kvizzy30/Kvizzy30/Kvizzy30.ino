/** Arduino, Esp32-CAM *************************************** Kvizzy30.ino ***
 * 
 *                    Kvizzy30 - управление встроенными светодиодами Esp32-CAM,  
 *                                           контрольным и вспышкой, через сайт
 *                    (модель нижнего уровня, стремящегося к умному, хозяйства)
 * 
 * v3.3.4, 05.01.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 31.05.2024
 * 
 *            Kvizzy: нижний уровень "умного хозяйства" - системы контроллеров, 
 *                  датчиков и исполнительных устройств, управляемых через сайт 
 * 
**/

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "AttachSNTP.h"
#include "jsonBase.h"       

// Вводим имя и пароль точки доступа
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

// Определяем директивы отладки
// #define tmr_TRACEMEMORY
// Определяем объект для синхронизации времени 
TAttachSNTP oSNTP;
// Определяем объект для работs с документом JSON
TJsonBase oJSON;

// Подключаем файлы обеспечения передачи и приёма сообщений через очередь                
#include "Kvizzy30_Message.h" // сообщения приложения  
#include <QueMessage.h>       // заголовочный файл класса TQueMessage                    
#include "QueChar.h"          // заголовочный файл класса TQueMessage                        
// Назначаем объекты работы с сообщениями через очередь                                   
TQueMessage queMessa(amessAPP,SizeMess,tmk_APP);    // для периферии                                     
TQue queState;                                      // для страницы State

#include "define_kvizzy.h"   // общие определения 
#include "common_kvizzy.h"   // общие функции  

// Подключаем задачи и деятельности
#include "Lead.h"            // 10-897 запрос контроллера на изменение состояний устройств
#include "State.h"           //  9-986 выборка сообщений о состоянии и отправка 
#include "Led33.h"           // обработка контрольного светодиода 
#include "Core.h"            // подключили обработку состояния процессоров 

// Определяем заголовок для объекта таймера
hw_timer_t *timer = NULL;
// Инициируем спинлок критической секции в обработчике таймерного прерывания
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Обработка прерывания от таймера
void IRAM_ATTR onTimer() 
{
   portENTER_CRITICAL_ISR(&timerMux);
   // Если флаги всех задач установлены в 1, 
   // то сбрасываем флаги задач и счетчик сторожевого таймера
   if (fwdtLed33==true && /*fwdtCore0==true &&*/ fwdtCore1==true && fwdtLoop==true && 
   fwdtLead==true && fwdtState==true && fwdtPrint==true) 
   {
      // Сбрасываем флаги задач
      fwdtLed33 = false;
      /*fwdtCore0 = false;*/
      fwdtCore1 = false;
      fwdtLoop  = false;
      fwdtLead = false;
      fwdtState = false;
      fwdtPrint = false;
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
// Обработка прерывания для вспышки при изменении состояние 4 контакта с LOW на HIGH (RISING).
void IRAM_ATTR onLedFlash()
{
}
// Начальная настройка: выделяем задачи 
// и обеспечиваем запуск прерывания от таймера периодически через 20 секунд
void setup() 
{
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

   // Проверяем системное время, если время еще не установлено, производим его 
   // синхронизацию по протоколу SNTP с серверами точного времени,
   oSNTP.Create();
   Serial.println("");
   
   // Создаем очередь сообщений на периферию                                                                   
   String inMess=queMessa.Create();                                                      
   // Если не получилось, сообщаем "Очередь не была создана и не может использоваться"    
   if (inMess==QueueNotCreate) Serial.println(QueueNotCreate);                           
   // Если очередь получилась, то отмечаем  "Очередь сформирована"                       
   else Serial.println(QueueBeformed);                                                   
   // Подключаем функцию передачи сообщения на периферию                                 
   queMessa.attachFunction(transmess);  

   // Создаем очередь сообщений на страницу State                                                                    
   inMess=queState.Create();                                                      
   // Если не получилось, сообщаем "Очередь не была создана и не может использоваться"    
   if (inMess==tQueueNotCreate) Serial.println(tQueueNotCreate);                         
   // Если очередь получилась, то отмечаем  "Очередь сформирована"                      
   else Serial.println(tQueueBeformed);                                                  
   // Подключаем функцию передачи сообщения на периферию                                
   queState.attachFunction(transState);                                                  

   // Отмечаем, что соединение с Wi-Fi установлено
   inMess=queMessa.Send(tmt_NOTICE,WifiEstablished,tmk_Queue);
   if (inMess!=isOk) Serial.println(inMess); 

   // Создаём объект и строку всего JSON-документа         
   oJSON.Create();

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
   //
   xTaskCreatePinnedToCore(
      vCore1,                 // Task function
      "Core1",                // Task name
      2048,                   // Stack size
      NULL,                   // Parameters passed to the task function
      6,                      // Priority
      NULL,                   // Task handle
      1);
   /*
   xTaskCreatePinnedToCore(
      vCore0,                 // Task function
      "Core0",                // Task name
      2048,                   // Stack size
      NULL,                   // Parameters passed to the task function
      6,                      // Priority
      NULL,                   // Task handle
      0);
   */
   // Выполнить регулярный (по таймеру) запрос контроллера на изменение   
   // состояний его устройств к странице Lead             
   xTaskCreatePinnedToCore(
      vLead,                  // Task function
      "Lead",                 // Task name
      2048,                   // Stack size
      NULL,                   // Parameters passed to the task function
      10,                     // Priority
      NULL,                   // Task handle
      1);
   // Выбрать накопившиеся json-сообщения о состоянии устройств контроллера 
   // и показаниях датчиков из очереди и отправить их на страницу State 
   xTaskCreatePinnedToCore(
      vState,                 // Task function
      "State",                // Task name
      3072,                   // Stack size
      NULL,                   // Parameters passed to the task function
      9,                      // Priority
      NULL,                   // Task handle
      1);
   // Выбрать из очереди и вывести сообщения в последовательный порт
   xTaskCreatePinnedToCore(
      vPrint,                 // Task function
      "Print",                // Task name
      2048,                   // Stack size
      NULL,                   // Parameters passed to the task function
      9,                      // Priority
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
}

// Инициируем прием кодов и заполнение строки
int incomingByte = 0; 
int incomingCode = 0; 
String data = "";

// Основной цикл
void loop() 
{
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
            isTrassState=true;
         }
         else if (incomingCode==disaTrassState) 
         {
            Serial.println("Отключение трассировки сообщений к State!"); 
            isTrassState=false;
         }
         // Готовим прием нового текста
         data = "";
      }
   }
   // Отмечаем флагом, что цикл задачи успешно завершен   
   fwdtLoop = true;
   // Ничего не делаем пол секунды
   vTaskDelay(500/portTICK_PERIOD_MS); 
}

// *********************************************************** Kvizzy30.ino ***
