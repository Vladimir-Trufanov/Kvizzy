/** Arduino-Esp32-CAM                             *** ex5-1-33_BasicWDT.ino ***
 * 
 *                Базовый пример сторожевого таймера задач в ESP32 board 5.1.33
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 * 
 * v1.0, 13.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 13.10.2024
 * 
 *    В примере используются мьютексы - функции для совместной работы нескольких задач 
 * с общими ресурсами. Фактически, мьютекс представляет собой способ ьлокировки и
 * предотвращения повреждения общих ("расшаренных", shared) ресурсов.
 *    Для этого каждому ресурсу назначается свой номер (токен) карты (мьютекс). Задача, 
 * которая забирает и владеет этой картой, может получить доступ к ресурсу. Остальные в 
 * это время должны ждать до тех пор, пока карта не возвратится (освободится). 
 *    Таким образом, только одна задача может получить доступ к ресурсу, а 
 * остальные должны ждать своего шанса.
**/

#include <esp_task_wdt.h>
#define WDT_TIMEOUT 10     // тайм-аут в секундах
esp_err_t ESP32_ERROR;     // возвращенное значение при инициализации TWDT

static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;

// Task A (high priority)
void doTaskA(void *parameters) 
{
  while (1) 
  {
    // Take mutex 1 (introduce wait to force deadlock)
    xSemaphoreTake(mutex_1, portMAX_DELAY);
    Serial.println("Task A took mutex 1");
    vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take mutex 2
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    Serial.println("Task A took mutex 2");

    // Critical section protected by 2 mutexes
    Serial.println("Task A doing some work");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Give back mutexes
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);

    // Wait to let the other task execute
    Serial.println("Task A going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task B (low priority)
void doTaskB(void *parameters) 
{
  while (1) 
  {

    // Take mutex 2 (introduce wait to force deadlock)
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    Serial.println("Task B took mutex 2");
    vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take mutex 1
    xSemaphoreTake(mutex_1, portMAX_DELAY);
    Serial.println("Task B took mutex 1");

    // Critical section protected by 2 mutexes
    Serial.println("Task B doing some work");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Give back mutexes
    xSemaphoreGive(mutex_1);
    xSemaphoreGive(mutex_2);

    // Wait to let the other task execute
    Serial.println("Task A going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() 
{
   Serial.begin(115200);

   /*
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
   */
   
   // Create mutexes before starting tasks
   mutex_1 = xSemaphoreCreateMutex();
   mutex_2 = xSemaphoreCreateMutex();

   // Create Task A (high priority), and Task B (low priority)
   xTaskCreate(doTaskA, "Task A", 1024, NULL, 2, NULL);
   xTaskCreate(doTaskB, "Task B", 1024, NULL, 1, NULL);
}

void loop() 
{
  // Execution should never get here
}
