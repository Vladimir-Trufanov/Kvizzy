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
 *                    
 * !!!
 * 1. Модуль ESP32 может обрабатывать до 32 прерываний на каждое ядро.
 * 
 * Словарь
 * 
 * Деятельность - выделенная часть основного цикла приложения, выполняющая
 *     отдельную задачу.
 * 
 * Тезисы проекта:
 * 
 * 1. Работают контрольный светодиод и вспышка. После запуска приложения контрольный светодиод мигает с интервалом в 1017 миллисекунд
 *    (текущий интервал в микросекундах горит на экране). Со стороны сайта можно изменить интервал мигания, отключить мигание, 
 *    включить мигание. Вспышка включается с сайта и горит почти 2 секунды и выключается.
 * 2. Есть 2 обработчика прерывания: для контрольного светодиода по смене состояния 33 пина (CHANGE) и для вспышки
 *    RISING - когда состояние 4 контакта изменяется с LOW на HIGH.
 * 3. 
**/

// Подключаем библиотеку для работы с HTTP-протоколом
#include <WiFi.h>
#include <HTTPClient.h>

// Вводим имя и пароль точки доступа
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

#include "define_kvizzy.h"   // подключили общие определения 
#include "define_json.h"     // подключили работу с документом JSON 
#include "common_kvizzy.h"   // подключили общие функции  

// Подключаем задачи и деятельности
#include "Led33.h"           // подключили обработку контрольного светодиода 
#include "Core.h"            // подключили обработку состояния процессоров 
#include "Serme.h"           // подключили общий вывод в последовательный порт
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
   if (fwdtLed33==true && fwdtCore0==true && fwdtCore1==true && fwdtLoop==true) 
   {
      // Сбрасываем флаги задач
      fwdtLed33 = false;
      fwdtCore0 = false;
      fwdtCore1 = false;
      fwdtLoop  = false;
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

// Обработка прерывания для контрольного светодиода при смене состояния 33 пина (CHANGE)
void IRAM_ATTR toggleLedWork()
{
   Serial.print("###");
   Serial.print(digitalRead(PinLedWork));
   Serial.println("###");
}

// Обработка прерывания для вспышки при изменении состояние 4 контакта с LOW на HIGH (RISING).
void IRAM_ATTR onLedFlash()
{
  
}


// Начальная настройка: выделяем четыре задачи (две на 0 процессоре, две на 1)
// и обеспечиваем запуск прерывания от таймера периодически через 3 секунды
void setup() 
{
   Serial.begin(115200);
   while (!Serial) continue;
   Serial.println("Последовательный порт работает!");

   // Проверяем пример
   // schastr();

   // Переводим контакты лампочек в режим вывода и подключаем обработку прерываний
   pinMode(PinLedWork,OUTPUT);    // контрольный светодиод
   attachInterrupt(PinLedWork,toggleLedWork,CHANGE);
   pinMode(PinLedFlash,OUTPUT);   // вспышка
   attachInterrupt(PinLedFlash,onLedFlash,RISING);

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

   // Подключаем задачу определение состояния контрольного светодиода ESP32-CAM 
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
   // Формируем общий json-документ
   sjson=thisController();

   Serial.println("");
   String str=getEsp32CAM(sjson);
   Serial.print("Контроллер: ");
   Serial.println(str);
}

// Основной цикл
void loop() 
{
   int currMillis = millis(); // время начала текущего цикла
   // Переключаем состояние контрольного светодиода через заданное количество микросекунд
   if ((currMillis < lastLedWork)||(currMillis - lastLedWork > millLedWork))
   { 
      lastLedWork = millis();  
      digitalWrite(PinLedWork, !digitalRead(PinLedWork));
   }
   // Отмечаем флагом, что цикл задачи успешно завершен   
   fwdtLoop = true;
   // Ничего не делаем пол секунды
   vTaskDelay(500/portTICK_PERIOD_MS);   
}

// *********************************************************** Kvizzy30.ino ***
