/** Arduino, Esp32-CAM *************************************** Kvizzy44.ino ***
 * 
 *         Kvizzy40 - исполнительный приложение умного хозяйства на контроллере 
 *          ESP32-CAM для управления светодиодом, снятия показаний температуры,
 *             влажности и формирования потока изображений наблюдаемого объекта
 * 
**/
#include <Arduino.h>
String ver="v4.4.0, 24.05.2025";                   // Автор:      Труфанов В.Е.
// Copyright © 2024 tve                            // Дата создания: 31.05.2024

#include <ArduinoOTA.h>   
#include "SD_MMC.h"            
#include <WiFi.h>
#include <HTTPClient.h>

// Определяем объект для постоянно хранимых переменных 
#include <Preferences.h>
Preferences Prefs;
// Определяем объект для синхронизации времени 
#include "AttachSNTP.h"
TAttachSNTP oSNTP;

// Подключаем файлы обеспечения передачи и приёма сообщений через очередь                
#include "kviMessage.h"       // сообщения приложения  
#include "QueMessage.h"       // заголовочный файл класса TQueMessage                    
// Назначаем объект работы с сообщениями через очередь                                   
TQueMessage queMessa(amessAPP,SizeMess,tmk_APP);    // для периферии                                     

#include "define_kvizzy.h"    // общие определения 
#include "common_kvizzy.h"    // общие функции  

// Определяем объект мьютекса - дескриптор для регулирования доступа к Http запросам
SemaphoreHandle_t HttpMutex = NULL;  

// Подключаем задачи
#include "kviPrint.h"         //  7-983  выборка из очереди и вывод сообщения на периферию
#include "kviStream.h"        // 10-2971 фотографирование и отправка изображения
#include "kviLed4.h"          // 12-1500
#include "kviLead.h"          // 13-997 
#include "kviDHT11.h"         //  6-2100

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
  && fwdtPrint==true
  && fwdtLead==true 
  //&& fwdtOTA==true 
  && fwdtLed4==true 
  && fwdtDHT11==true 
  && fwdtStream==true) 
  {
    // Сбрасываем флаги задач
    fwdtLoop  = false;
    fwdtPrint = false;
    fwdtLead = false;
    fwdtLed4 = false;
    fwdtDHT11 = false;
    fwdtStream = false;
    //fwdtOTA = false;
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
// Определяем директивы отладки
// #define tmr_TRACEMEMORY
// Определяем объект для работs с документом JSON
//TJsonBase oJSON;

// ****************************************************************************
// *                         Начальная настройка                              *
// ****************************************************************************
void setup() 
{
  // Включаем последовательный порт
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("Последовательный порт работает!");
  // Инициируем пространство постоянно хранимых переменных
  iniPMem(Prefs); 
  // Подключаемся к Wi-Fi сети
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  Serial.print("Соединяемся с Wi-Fi.");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("К Wi-Fi сети подключились!");
  // Запускаем OTA
  ArduinoOTA.begin(); 
  Serial.println("OTA включено!");
  // Проверяем системное время, если время еще не установлено, производим его 
  // синхронизацию по протоколу SNTP с серверами точного времени,
  oSNTP.Create();
  // Включаем однобитовый, медленный, без вспышки режим работы с SD-картой
  if (!SD_MMC.begin("/sdcard", true))
  {
    Serial.println("\nSD-карта не смонтирована");
    return;
  }
  else Serial.println("\nSD карта смонтирована");
  // Гасим вспышку (вдруг она горит)
  analogWrite(LED_PIN_4, 0);
  // Подключаем устройство
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE)
  {
    Serial.println("SD карта не подключена");
    return;
  }
  else Serial.println("SD карта подключена");

  // Создаем мьютекс доступа к Http-запросам
  HttpMutex = xSemaphoreCreateMutex();  


  // Подключаем задачу по выборке из очереди и отправке сообщения на периферию
  xTaskCreatePinnedToCore(
    vPrint,                 // Task function
    "Print",                // Task name
    2048,                   // Stack size
    NULL,                   // Parameters passed to the task function
    7,                      // Priority
    NULL,                   // Task handle
    1);
  // Подключаем задачу по фотографированию и отправке Base24-изображения на страницу Stream
  xTaskCreatePinnedToCore(
    vStream,                // Task function
    "Stream",               // Task name
    //8480,                 // Stack size
    24576,
    NULL,                   // Parameters passed to the task function
    10,                     // Priority
    NULL,                   // Task handle
    1);
  // Подключаем задачу управления 4 сведодиодом ESP32-CAM в режиме "горит - не горит"
  // и передачу данных на страницу сайта State
  xTaskCreatePinnedToCore(
    vLed4,                  // Task function
    "Led4",                 // Task name
    4096,                   // Stack size
    NULL,                   // Parameters passed to the task function
    12,                     // Priority
    NULL,                   // Task handle
    1); 
  // Обеспечиваем взаимодействие датчика DHT11 c контроллером ESP32-CAM по GPIO12 
  // и передачу данных о температуре и влажности на страницу сайта State 
  xTaskCreatePinnedToCore(
    vDHT11,                 // Task function
    "DHT11",                // Task name
    4096,                   // Stack size
    NULL,                   // Parameters passed to the task function
    6,                      // Priority
    NULL,                   // Task handle
    1); 
  // Выполнить регулярный (по таймеру) запрос контроллера на изменение   
  // состояний его устройств к странице Lead             
  xTaskCreatePinnedToCore(
    vLead,                  // Task function
    "Lead",                 // Task name
    4096,                   // Stack size
    NULL,                   // Parameters passed to the task function
    13,                      // Priority
    NULL,                   // Task handle
    1);
 
  /*
  // Создаём объект и строку всего JSON-документа         
   oJSON.Create();
   Serial.println("");
  */
  
  // Создаем очередь сообщений на периферию                                                                   
  String inMess=queMessa.Create();                                                      
  // Если не получилось, сообщаем "Очередь не была создана и не может использоваться"    
  if (inMess==QueueNotCreate) Serial.println(QueueNotCreate);                           
  // Если очередь получилась, то отмечаем  "Очередь сформирована"                       
  else {Serial.print(QueueBeformed); Serial.println(" для сообщений на периферию");}                                                   
  // Подключаем функцию передачи сообщения на периферию                                 
  queMessa.attachFunction(transPrint);  

  // Отмечаем, что соединение с Wi-Fi установлено
  inMess=queMessa.Send(tmt_NOTICE,WifiEstablished,tmk_Queue);
  if (inMess!=isOk) Serial.println(inMess); 

  Serial.println("");
  
  /*
  // Определяем дополнительную задачу
  xTaskCreatePinnedToCore (
    vOTA,           // название функции, которая будет запускаться, как параллельная задача
    "OTA",          // название задачи
    8192,           // размер стека в байтах. Задача будет использовать этот объем памяти, когда 
                    // ей потребуется сохранить временные переменные и результаты. Для задач с 
                    // большим объемом памяти потребуется больший размер стека.
    NULL,           // указатель на параметр, который будет передан новой задаче. 
                    // NULL, если параметр не передаётся.
    1,             // приоритет задачи
    NULL,           // дескриптор или указатель на задачу. Его можно использовать для вызова задачи.
                    // Если это не требуется, то NULL.
    1               // идентификатор ядра процессора, на котором требуется запустить задачу. 
                    // У ESP32 есть два ядра, обозначенные как 0 и 1.
  );
  */


  // Создаём объект таймера, устанавливаем его частоту отсчёта (1Mhz)
  timer = timerBegin(1000000);
  // Подключаем функцию обработчика прерывания от таймера - onTimer
  timerAttachInterrupt(timer, &onTimer);
  // Настраиваем таймер: интервал перезапуска - 20 секунд (20000000 микросекунд),
  // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
  // раз (четвертый параметр = 0) 
  timerAlarm(timer, 40000000, true, 0);
  Serial.println("Установлен тайм-аут сторожевого таймера 40 сек.");

  // Создаём объект таймера вспышки, устанавливаем его частоту отсчёта (1Mhz)
  timerLed4 = timerBegin(1000000);
  // Подключаем функцию обработчика прерывания от таймера - onTimer
  timerAttachInterrupt(timerLed4, &onTimerLed4);
  // Настраиваем таймер: интервал перезапуска - 5 секунд (для первого запуска, далее по расчету),
  // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
  // раз (четвертый параметр = 0) 
  timerAlarm(timerLed4, 5000000, true, 0);
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
  // Serial.println("*** "+ver+" ***");
  // Запускаем обработку запроса на обновление кода
  ArduinoOTA.handle(); 
  
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

// *********************************************************** Kvizzy44.ino ***
