/** Arduino-Esp32-CAM                             *** ex5-1-33_BasicWDT.ino ***
 * 
 *                Базовый пример сторожевого таймера задач в ESP32 board 5.1.33
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 * 
 * v1.1, 12.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.10.2024
 * 
 * Ключевой момент — установить задержку минимум в 1 мс после выполнения 
 * esp_task_wdt_reset:
**/

/* ========================================== Устаревшая реализация примера ===
#include <esp_task_wdt.h>
#define WDT_TIMEOUT 3                   // WDT Timeout in seconds
void setup() 
{
   Serial.begin(115200);
   Serial.println("Setup started.");
   delay(2000);
   esp_task_wdt_init(WDT_TIMEOUT, true); // Initialize ESP32 Task WDT
   esp_task_wdt_add(NULL);               // Subscribe to the Task WDT
}
void loop() 
{
   Serial.println("LOOP started ! ");
   for (int i = 0; i <= 10; i++) 
   {
      Serial.print("Task: ");
      Serial.println(i);
      delay(1000);
      // Kick the dog
      esp_task_wdt_reset();
   }
   while (1) 
   { 
      Serial.println("MCU hang event!!!");
   }
}
*/

#include <esp_task_wdt.h>
#define WDT_TIMEOUT 3      // тайм-аут в секундах
esp_err_t ESP32_ERROR;     // возвращенное значение при инициализации TWDT

void setup() 
{
   Serial.begin(115200);
   Serial.println("Setup started.");
   delay(2000);
   // Отменяем подписку на незанятые задачи и деинициализируем таймер отслеживания задач TWDT
   esp_task_wdt_deinit();
   // Конфигурируем структуру таймера контроля задач (TWDT):
   esp_task_wdt_config_t wdt_config = 
   {
      .timeout_ms = WDT_TIMEOUT * 1000,                 // длительность тайм-аута в мс
      .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,  // битовая маска для всех ядер
      .trigger_panic = true                             // включить перезагрузку ESP32
   };
   // Инициализируем таймер контроля задач (TWDT)
   ESP32_ERROR = esp_task_wdt_init(&wdt_config);
   // Подписываем текущую задачу под наблюдение TWDT
   esp_task_wdt_add(NULL);  
}

void loop() 
{
   Serial.println("LOOP started.");
   // Сбрасывем сторожевой таймер 10 секунд
   for (int i = 0; i <= 10; i++) 
   {
      Serial.print("Task: ");
      Serial.println(i);
      delay(1000);
      // Сбрасываем таймер контроля задач (TWDT) от имени текущей задачи
      // (kick the dog - "пинаем собаку")
      esp_task_wdt_reset();
      // Делаем задержку в, как минимум, 1 мсек
      delay(1);  
   }
   // Уходим в бесконечный цикл без сброса TWDT
   while (1) 
   {
      Serial.println("MCU hang event!!!");
   }
}

// ************************************************** ex5-1-33_BasicWDT.ino ***

