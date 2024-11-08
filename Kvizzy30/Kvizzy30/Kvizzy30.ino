/** Arduino, Esp32-CAM *************************************** Kvizzy30.ino ***
 * 
 * Kvizzy30 - модельное программное обеспечение для Esp32-CAM и двух встроенных
 *    светодиодов: контрольного и вспышки (нижний уровень стремящегося к умному
 *                                                                   хозяйства)
 * 
 * v3.3, 08.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 31.05.2024
 * 
 *           Kvizzy - система контроллеров, датчиков и исполнительных устройств 
 *                    моего, стремящегося к умному, хозяйства (нижний уровень).
**/

// Подключаем библиотеку для работы с HTTP-протоколом
#include <WiFi.h>
#include <HTTPClient.h>

// Вводим имя и пароль точки доступа
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

// Определяем состояния светодиода с обратной логикой
//#define inHIGH LOW
//#define inLOW  HIGH 

#include "define_kvizzy.h"   // подключили общие определения 
#include "common_kvizzy.h"   // подключили общие функции  

#include "Led33.h"           // подключили обработку контрольного светодиода 

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
   if (fwdtLed33 == true && flag[1] == 1 && flag[2] == 1 && flag[3] == 1) 
   {
      // Сбрасываем флаги задач
      fwdtLed33 = false;
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

   // Подключаемся к Wi-Fi сети
   WiFi.begin(ssid, password);
   Serial.print("Соединяемся с Wi-Fi .");
   while (WiFi.status() != WL_CONNECTED) 
   {
      delay(500);
      Serial.print(".");
   }
   Serial.println(" ");
   Serial.println("Соединение с Wi-Fi установлено");

   // Сделан низкий приоритет, так как часто (и занимается посл.порт на прием)
   xTaskCreatePinnedToCore(
      vTastes,                // Task function
      "Tastes",               // Task name
      1024,                   // Stack size
      NULL,                   // Parameters passed to the task function
      1,                      // Priority
      NULL,                   // Task handle
      1); 

   // Подключаем задачу определение состояния контрольного светодиода ESP32-CAM 
   // ("горит - не горит") и передачу данных на страницу сайта State  
   xTaskCreatePinnedToCore(
      vLed33,                 // Task function
      "Led33",                // Task name
      2048,                   // Stack size
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

   xTaskCreatePinnedToCore(
      vCore0,                 // Task function
      "Core0",                // Task name
      2048,                   // Stack size
      NULL,                   // Parameters passed to the task function
      6,                      // Priority
      NULL,                   // Task handle
      0);

   // Создаём объект таймера, устанавливаем его частоту отсчёта (1Mhz)
   timer = timerBegin(1000000);
   // Подключаем функцию обработчика прерывания от таймера - onTimer
   timerAttachInterrupt(timer, &onTimer);
   // Настраиваем таймер: интервал перезапуска - 20 секунд (20000000 микросекунд),
   // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
   // раз (четвертый параметр = 0) 
   timerAlarm(timer, 20000000, true, 0);
   
   sjson=thisController();

   Serial.println("");
   String str=getEsp32CAM(sjson);
   Serial.print("Контроллер: ");
   Serial.println(str);

   //ssetup(sjson); 
   
}

// Основной цикл
void loop() 
{
   // выполняем проверку подключения к беспроводной сети
   if ((WiFi.status() == WL_CONNECTED)) 
   {
      // Cоздаем объект для работы с HTTP
      HTTPClient http;
      // Подключаемся к веб-странице
      // http.begin("https://kwinflatht.nichost.ru/State");
      http.begin("https://doortry.ru/State/");  // 2024-11-07 получилось
      // http.begin("www.doortry.ru/State/") = "Ошибка HTTP-запроса";
      
      // Делаем GET запрос
      int httpCode = http.GET();
      // Проверяем успешность запроса
      if (httpCode > 0) 
      {
         // Выводим ответ сервера
         String ContentPage = http.getString();
         Serial.println(httpCode);
         Serial.println(ContentPage);
      }
      else 
      {
         Serial.println("Ошибка HTTP-запроса");
      }
      // Освобождаем ресурсы микроконтроллера
      http.end();
   }
   delay(15000);
}

// Сделано часто, так как считываем целое число из последовательного порта
// и мигаем лампочками
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
         if (ii>0) iCreateSit=ii;
         vTaskDelay(100/portTICK_PERIOD_MS);   
      }
      // Мигаем лампочкой
      if (iCreateSit == disaflashing33) digitalWrite(LedWorkEsp32Cam,inLOW);
      else
      {
         int mitLed33i=millis();
         if ((mitLed33i-mitLed33)>2016)
         {
            digitalWrite(LedWorkEsp32Cam,!digitalRead(LedWorkEsp32Cam));
            mitLed33 = mitLed33i;
         }
         if (iCreateSit == loopingTastes) MimicMCUhangEvent("Tastes");   
      }
      flag[1] = 1;
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
      if (iCreateSit == 3) MimicMCUhangEvent("Core1");   
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
      if (iCreateSit == loopingCore0) MimicMCUhangEvent("Core0");   
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
