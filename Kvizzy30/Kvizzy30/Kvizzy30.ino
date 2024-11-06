/** Arduino, Esp32-CAM *************************************** Kvizzy30.ino ***
 * 
 * Kvizzy30 - модельное программное обеспечение для Esp32-CAM и двух встроенных
 *    светодиодов: контрольного и вспышки (нижний уровень стремящегося к умному
 *                                                                   хозяйства)
 * 
 * v3.2, 05.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 31.05.2024
 * 
 *           Kvizzy - система контроллеров, датчиков и исполнительных устройств 
 *                    моего, стремящегося к умному, хозяйства (нижний уровень).
 * 
**/

// Подключаем библиотеку для работы с HTTP-протоколом
#include <WiFi.h>
#include <HTTPClient.h>

// Вводим имя и пароль точки доступа
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

// Размещаем JSON-документ
#include <ArduinoJson.h>
JsonDocument doc;


// Определяем состояния светодиода с обратной логикой
#define inHIGH LOW
#define inLOW  HIGH 


#include "define_kvizzy.h"   // подключили общие определения 
#include "common_kvizzy.h"   // подключили общие функции  


// Определяем заголовок для объекта таймера
hw_timer_t *timer = NULL;
// Инициируем спинлок критической секции в обработчике таймерного прерывания
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
// Определяем число, которое будет считываться в основном цикле
// с последовательного порта
volatile int inumber;
// Пины для мигания лампочек
#define LedWorkEsp32Cam  33     // контакт рабочего светодиода
//
volatile int mitLed33=millis();
volatile int mittLed33=millis();
volatile int mitMimic=millis();

// Определяем задачи и их флаги
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vCore1(void *pvParameters);
void vCore0(void *pvParameters);
int flag[] = {0, 0, 0, 0};

// Обработка прерывания от таймера
void IRAM_ATTR onTimer() 
{
   portENTER_CRITICAL_ISR(&timerMux);
   // Если флаги всех задач установлены в 1, 
   // то сбрасываем флаги задач и счетчик сторожевого таймера
   if (flag[0] == 1 && flag[1] == 1 && flag[2] == 1 && flag[3] == 1) 
   {
      // Сбрасываем флаги задач
      flag[0] = 0;
      flag[1] = 0;
      flag[2] = 0;
      flag[3] = 0;
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

// Начальная настройка: выделяем четыре задачи (две на 0 процессоре, две на 1)
// и обеспечиваем запуск прерывания от таймера периодически через 3 секунды
void setup() 
{
   Serial.begin(115200);
   while (!Serial) continue;
   Serial.println("Последовательный порт работает!");

   // Переводим контакты лампочек в режим вывода
   pinMode(LedWorkEsp32Cam,OUTPUT);    // "работает"
   digitalWrite(LedWorkEsp32Cam,true);

  // подключаемся к Wi-Fi сети
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Соединяемся с Wi-Fi..");
  }
  Serial.println("Соединение с Wi-Fi установлено");


   xTaskCreatePinnedToCore(
      vTask1,                 // Task function
      "Task1",                // Task name
      2048,                   // Stack size
      NULL,                   // Parameters passed to the task function
      10,                      // Priority
      NULL,                   // Task handle
      1); //ARDUINO_RUNNING_CORE);

   xTaskCreatePinnedToCore(
      vTastes,                // Task function
      "Tastes",               // Task name
      1024,                   // Stack size
      NULL,                   // Parameters passed to the task function
      1,                      // Priority
      NULL,                   // Task handle
      1); 

   xTaskCreatePinnedToCore(
      vCore1,                 // Task function
      "Core1",                // Task name
      2048,                   // Stack size
      NULL,                   // Parameters passed to the task function
      2,                      // Priority
      NULL,                   // Task handle
      1);

   xTaskCreatePinnedToCore(
      vCore0,                 // Task function
      "Core0",                // Task name
      2048,                   // Stack size
      NULL,                   // Parameters passed to the task function
      3,                      // Priority
      NULL,                   // Task handle
      1);

   // Создаём объект таймера, устанавливаем его частоту отсчёта (1Mhz)
   timer = timerBegin(1000000);
   // Подключаем функцию обработчика прерывания от таймера - onTimer
   timerAttachInterrupt(timer, &onTimer);
   // Настраиваем таймер: интервал перезапуска - 20 секунд (20000000 микросекунд),
   // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
   // раз (четвертый параметр = 0) 
   timerAlarm(timer, 120000000, true, 0);

   sjson=thisController();

   Serial.println("");
   String str=getEsp32CAM(sjson);
   Serial.print("Контроллер: ");
   Serial.println(str);

   ssetup(sjson); 

   /*
   str=getDHT22(sjson);
   Serial.print("getDHT22: ");
   Serial.println(str);
   */
}

// Основной цикл
void loop() 
{
}

// Имитируем событие зависания процессора
void MimicMCUhangEvent(String NameTask)
{
   while (true)
   {
      int mitMimici=millis();
      if ((mitMimici-mitMimic)>1005)
      {
         Serial.print(NameTask);
         Serial.println(": зависание процессора!!!");
         mitMimic = mitMimici;
      }
   }
}


String getLed33(String sjson) 
{
   JsonDocument filter;
   filter["nicctrl"] = true; 
   filter["led33"][0]["typedev"]= true;
   filter["led33"][0]["status"]= true;
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   int LedStatus=digitalRead(LedWorkEsp32Cam);
   if (LedStatus==inHIGH) doc["led33"][0]["status"]="inHIGH";
   else doc["led33"][0]["status"]="inLOW";
   String str = "";
   serializeJson(doc,str);
   return str;
}

void vTask1(void* pvParameters) 
{
   for (;;)
   {
      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (inumber == 1) MimicMCUhangEvent("Task1");   

      String str=getLed33(sjson);
      Serial.print("getLed33: ");
      Serial.println(str);
      vTaskDelay(1007/portTICK_PERIOD_MS);   

      flag[0] = 1;
   }
}
void vTastes(void* pvParameters) 
{
   for ( ;; )
   {
      // Считываем с последовательного порта целое число
      // (так как в зависимости от окружения за целым числом может следовать нулевое значение,
      // то отсекаем 0)
      if (Serial.available() > 0) 
      {
         int ii=Serial.parseInt();
         if (ii>0) inumber=ii;
         delay(100);
      }
      
      // Мигаем лампочкой
      int mitLed33i=millis();
      if ((mitLed33i-mitLed33)>2016)
      {
         digitalWrite(LedWorkEsp32Cam,!digitalRead(LedWorkEsp32Cam));
         mitLed33 = mitLed33i;
      }
      flag[1] = 1;
      if (inumber == 2) MimicMCUhangEvent("Tastes");   
   }
}
void vCore1(void* pvParameters) 
{
   for ( ;; )
   {
      String str=getCoreX(sjson);
      Serial.print("getCore1: ");
      Serial.println(str);
      vTaskDelay(6000/portTICK_PERIOD_MS);   // 6 секунд задача
      flag[2] = 1;
      if (inumber == 3) MimicMCUhangEvent("Core1");   
   }
}
void vCore0(void* pvParameters) 
{
   for ( ;; )
   {
      String str=getCoreX(sjson,"Core0");
      Serial.print("getCore0: ");
      Serial.println(str);

      vTaskDelay(5000/portTICK_PERIOD_MS);   // 5 секунд задача 
      flag[3] = 1;
      if (inumber == 4) MimicMCUhangEvent("Core0");   
   }
}

void ssetup(String g) 
{
  // JSON input string.
  // const char* json = "12{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  const char* json = g.c_str();
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Fetch values.
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  const char* namectrl = doc["namectrl"];
  double minstack = doc["core0"][0]["minstack"];
  int DHT22status = doc["DHT22"][0]["status"];
  // Print values.
  Serial.println(namectrl);
  Serial.println(minstack,2);
  Serial.println(DHT22status);
}

// ************************************************* ex3-0-6-5-GreatWDT.ino ***
