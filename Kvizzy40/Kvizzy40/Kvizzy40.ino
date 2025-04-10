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

// для проверки фотогр ------------------------------------------------------

// Инициируем объект преобразования изображения base64
#include <base64.h>
base64 b;


#include "OV2640.h"
#include "SD_MMC.h"            
#include <EEPROM.h>        

// Определяем число байт для хранения в постоянной памяти
#define EEPROM_SIZE 1
// Определяем пин вспышки
#define BUILTIN_LED 4
// Инициализируем переменную, обеспечивающую генерацию названий фотографий:
// picture1.jpg, picture2.jpg ...
int pictureNumber = 0;
// Инициируем объект для фотографирования
OV2640 cam;
// Определяем режим записи на SD
#define SD_MODE_NORMAL 4  // обычный по 4 контактам со вспышкой
#define SD_MODE_1BIT   1  // однобитовый, медленный, без вспышки

#define SD_MODE SD_MODE_1BIT  
// для проверки фотогр ------------------------------------------------------




#include <WiFi.h>
#include <HTTPClient.h>

// Определяем объект для синхронизации времени 
#include "AttachSNTP.h"
TAttachSNTP oSNTP;

// Подключаем файлы обеспечения передачи и приёма сообщений через очередь                
#include "Kvizzy40_Message.h" // сообщения приложения  
#include "QueMessage.h"       // заголовочный файл класса TQueMessage                    
#include "QueChar.h"          // заголовочный файл класса TQueChar                        
// Назначаем объекты работы с сообщениями через очередь                                   
TQueMessage queMessa(amessAPP,SizeMess,tmk_APP);    // для периферии                                     
TQue queState;                                      // для страницы State

#include "define_kvizzy.h"   // общие определения 
#include "common_kvizzy.h"   // общие функции  

// Подключаем задачи
#include "kviPrint.h"        // 7-983  выборка из очереди и вывод сообщения на периферию
#include "kviStream.h"       // 8-2971 фотографирование и отправка изображения

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
    /* 
    && fwdtLed33==true &&  
    fwdtLead==true && fwdtState==true 
    */
  && fwdtStream==true) 
  {
    // Сбрасываем флаги задач
    fwdtLoop  = false;
    fwdtPrint = false;
      /*
      fwdtLed33 = false;
      fwdtLead = false;
      fwdtState = false;
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

#include "jsonBase.h"       

// Определяем директивы отладки
// #define tmr_TRACEMEMORY
// Определяем объект для работs с документом JSON
TJsonBase oJSON;


#include "define_kvizzy.h"   // общие определения 
#include "common_kvizzy.h"   // общие функции  

// Подключаем задачи и деятельности
#include "Lead.h"            //  9-897 запрос контроллера на изменение состояний устройств
#include "State.h"           //  8-986 выборка сообщений о состоянии и отправка 
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
  Serial.print("Соединяемся с Wi-Fi.");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("К Wi-Fi сети подключились!");
  // Проверяем системное время, если время еще не установлено, производим его 
  // синхронизацию по протоколу SNTP с серверами точного времени,
  oSNTP.Create();

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


// для проверки фотогр ------------------------------------------------------

// Инициируем постоянную память заданного размера
  EEPROM.begin(EEPROM_SIZE);
  // Назначаем номер первой сохраняемой фотографии
  EEPROM.write(0, 254);
  EEPROM.commit();
  Serial.println("Инициирована постоянная память\n");

  Serial.println("Подключаем SD-карту");
  // Настраиваем обычный или однобитовый режим работы
  #if (SD_MODE==SD_MODE_NORMAL)
    if(!SD_MMC.begin())
  #else
    if (!SD_MMC.begin("/sdcard", true))
  #endif
  {
    Serial.println("SD-карта не смонтирована");
    return;
  }
  else Serial.println("SD карта смонтирована");
  // Гасим вспышку в однобитовом режиме
  #if (SD_MODE==SD_MODE_1BIT)
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
  #endif 
  // Подключаем устройство
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE)
  {
    Serial.println("SD карта не подключена");
    return;
  }
  else Serial.println("SD карта подключена");
  
  // Инициируем камеру 
  cam.init(esp32cam_aithinker_config);
  // Определяем дополнительную задачу
  xTaskCreatePinnedToCore (
    instream,       // название функции, которая будет запускаться, как параллельная задача
    "instream",     // название задачи
    48480,          // размер стека в байтах. Задача будет использовать этот объем памяти, когда 
                    // ей потребуется сохранить временные переменные и результаты. Для задач с 
                    // большим объемом памяти потребуется больший размер стека.
    NULL,           // указатель на параметр, который будет передан новой задаче. 
                    // NULL, если параметр не передаётся.
    9,              // приоритет задачи
    NULL,           // дескриптор или указатель на задачу. Его можно использовать для вызова задачи.
                    // Если это не требуется, то NULL.
    1               // идентификатор ядра процессора, на котором требуется запустить задачу. 
                    // У ESP32 есть два ядра, обозначенные как 0 и 1.
  );

// для проверки фотогр ------------------------------------------------------












  // Создаём объект таймера, устанавливаем его частоту отсчёта (1Mhz)
  timer = timerBegin(1000000);
  // Подключаем функцию обработчика прерывания от таймера - onTimer
  timerAttachInterrupt(timer, &onTimer);
  // Настраиваем таймер: интервал перезапуска - 20 секунд (20000000 микросекунд),
  // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
  // раз (четвертый параметр = 0) 
  timerAlarm(timer, 20000000, true, 0);
  Serial.println("Установлен тайм-аут сторожевого таймера 20 сек.");
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

// ****************************************************************************
// *              Выполнять фотографирование в некоторых циклах               *
// * !!! Если задача завершится (не будет циклится),контроллер перезагрузится *
// ****************************************************************************
void instream (void* pvParameters) 
{

  time_t nTime;      // время отправки фрэйма (секунда с начала эпохи)
  time_t nTimeOld;   // время отправки предыдущего фрэйма
  int nFrame=0;      // номер кадра в секунде

  // Инициируем метки времени
  time(&nTime);                    // время отправки фрэйма (секунда с начала эпохи)
  time(&nTimeOld);                 // время отправки предыдущего фрэйма

  while (1) 
  {
    // Пересчитываем время и номер кадра
    time(&nTime); 
    if (nTime==nTimeOld) nFrame=nFrame+1;
    else {nTimeOld=nTime; nFrame=0;}

    cam.run();
    size_t SizeFR = cam.getSize();
    //const char * bb = callphoto(cam.getfb(),SizeFR);
    String imgname = callphoto(cam.getfb(),SizeFR);
    Serial.print("imgname = "); Serial.println(imgname);

    //sendhttp(nTime, nFrame, *bb);
    //sendhttp(nTime, nFrame, "/vga640x480.jpg");
    sendhttp(nTime, nFrame, imgname);
    //sendhttp(nTime, nFrame, "/fil1.jpg");
    //sendhttp(nTime, nFrame, callphoto(cam.getfb(),cam.getSize()));
    vTaskDelay(1200/portTICK_PERIOD_MS);
  }
}
int nCikl=0;
//const char * callphoto(uint8_t *payload, uint16_t len)
String callphoto(uint8_t *payload, uint16_t len)
{
  String path;

  nCikl++;
  Serial.print("Цикл: "); Serial.println(nCikl);

  if ((nCikl>1) && (nCikl<15))
  {
    pictureNumber = EEPROM.read(0) + 1;
    // Определяем имя своего файла фотографии в каталоге карты microSD
    path = "/picture" + String(pictureNumber) +".jpg";
    // Сохраняем фотографию на карту microSD
    fs::FS & fs = SD_MMC; 
    Serial.printf("Название фото: %s\n", path.c_str());
    File file = fs.open(path.c_str(), FILE_WRITE);
    if(!file)
    {
      Serial.println("Ошибка открытия файла в режиме записи");
    } 
    else 
    {
      file.write(payload, len); 
      Serial.printf("Сохранено изображение: %s\n", path.c_str());
      // Cохраняем текущий номер снимка во флэш-памяти, 
      // чтобы отслеживать количество сделанных фотографий.
      EEPROM.write(0, pictureNumber);
      EEPROM.commit();
    }
    file.close();
  }
  //return path.c_str();
  return path;
}

//String readFile(const char *path) 
String readFile(String path) 
{
  char *str;

  Serial.printf("Reading file: %s\n", path);
  // ----Сохраняем фотографию на карту microSD
  fs::FS & fs = SD_MMC; 

  File file = fs.open(path);
  if (!file) 
  {
    Serial.println("Failed to open file for reading");
    return "Failed to open file for reading";
  }

  Serial.print("Read from file, rlen = ");
  uint8_t bufi[20000];
  int rlen = file.available();
  Serial.println(rlen);
  file.read(bufi, rlen); 
  bufi[rlen]=0;

  //char *str;
  str = (char*)bufi;

  String stringOne = String(str);
  //Serial.println(stringOne);

  /*
  while (file.available()) 
  {
    char ch = file.read();    // read the first character
    Serial.print(ch);
    //Serial.write(file.read());
  }
  */
  //Serial.println(" ");
  return stringOne;
}

//void sendhttp(time_t nTime, int nFrame, const char *path) 
void sendhttp(time_t nTime, int nFrame, String path) 
{
  String inMess;
  if ((WiFi.status() == WL_CONNECTED)) 
  {
    HTTPClient http;
    //http.begin(ehttp);
    http.begin("http://probatv.ru/Stream40/");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    //String frame="Pirelli";
    //String frame=readFile(path);
    String frame="data:image/jpeg;base64,"+b.encode(readFile(path));
    //frame="data:image/jpeg;base64,"+frame;
    String queryString = "src="+frame;      
    // Добавляем время с начала эпохи в параметры
    String stime="&time="+String(nTime);
    queryString=queryString+stime;
    // Добавляем номер кадра в параметры
    String sframe="&frame="+String(nFrame);
    queryString=queryString+sframe;
    //Serial.println("queryString: "); Serial.println(queryString);
    
    int tQuery_httpCode = http.POST(queryString); 
    //tQuery.httpCode = http.POST("");  
    if (tQuery_httpCode > 0) 
    {
      // Если запрос успешно отправлен
      if (tQuery_httpCode == HTTP_CODE_OK) 
      {
        inMess = http.getString();
        Serial.println("Запрос успешно отправлен: "); Serial.println(inMess);
        //Serial.print("queryString: "); Serial.println(queryString);
      }
      // Если ошибка после того, как HTTP-заголовок был отправлен
      // и заголовок ответа сервера был обработан
      else 
      {
        // Если сообщение о ненайденной странице, указываем её
        if (tQuery_httpCode==404) inMess="404";
        else inMess="Не 404";
        Serial.print("Запрос c ошибкой "); Serial.println(inMess);
      }
    }
    // Если ошибка при отправке POST-запроса
    //    Ошибка POST-запроса: "read Timeout"       - "истекло время ожидания чтения"
    //    Ошибка POST-запроса: "connection refused" - "В соединении отказано"
    else 
    {
      inMess=http.errorToString(tQuery_httpCode);
      Serial.printf("Ошибка POST-запроса: %s\n", inMess.c_str());
    }
    http.end();
  }
  // Если "Нет подключения к WiFi перед передачей POST-запроса"
  else
  {
    inMess="Нет подключения к WiFi перед передачей POST-запроса";
    Serial.println(inMess);
  }
}


// *********************************************************** Kvizzy40.ino ***
