/** KRUTJAK-MASTER-PRO_MINI                                    *** mk32.ino ***
 * 
 * mk32 - Управляющая система паровозика "КРУТЯК" 
 * 
 * v3.4, 04.05.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

// ИСКР - исполнительная система паровозика "КРУТЯК"
// УПС  - управляющая система

/** 
 * Таймер сторожевого механизма в ESP32 board 3.0x сильно отличается от предыдущей
 * реализации. Вот простой пример его реализации (в Arduino IDE).
 * 
 * 
 * Ключевой момент — установить задержку минимум в 1 мс после esp_task_wdt_reset:
 *    esp_task_wdt_reset();
 *    delay(1);  
 * 
 * Здесь после примера, который нужно протестировать:
 * - время ожидания установлено на 25 секунд
 * - в цикле 20 секунд. Сброс выполняется в течение первых 10 секунд, затем в течение 10 секунд сброс не выполняется.
 * - после 60 секунд без сброса система выйдет из строя и перезапустится через 75 секунд.
**/

/*
#include <esp_task_wdt.h>
#define WDT_TIMEOUT 25  // Тайм-аут в секундах [4](https://f1atb.fr/forum_f1atb/thread-174.html)
esp_err_t ESP32_ERROR;
int i = 0;
int last = millis();

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("Configuring WDT...");
    Serial.print("Watchdog Timeout (in seconds) set to : ");
    Serial.println(WDT_TIMEOUT);
    esp_task_wdt_deinit();

    // Конфигурация таймера сторожевого пса [4](https://f1atb.fr/forum_f1atb/thread-174.html)
    esp_task_wdt_config_t wdt_config = {
        .timeout_ms = WDT_TIMEOUT * 1000,  // Конвертация в миллисекунды [4](https://f1atb.fr/forum_f1atb/thread-174.html)
        .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,  // Битовая маска всех ядер [4](https://f1atb.fr/forum_f1atb/thread-174.html)
        .trigger_panic = true  // Включить панику для перезагрузки ESP32 [4](https://f1atb.fr/forum_f1atb/thread-174.html)
    };

    ESP32_ERROR = esp_task_wdt_init(&wdt_config);
    Serial.println("Last Reset : " + String(esp_err_to_name(ESP32_ERROR)));
    esp_task_wdt_add(NULL);  // Добавить текущую нить в наблюдение за TWDT [4](https://f1atb.fr/forum_f1atb/thread-174.html)
}
void loop() {
    if (millis() - last >= 1000) {  // Задача каждую секунду
        last = millis();
        Serial.print(i);
        Serial.print("s : ");
        i++;
        if (i % 20 <= 10 && i<60) {  // Сброс только в первые 10 секунд первой минуты
            Serial.println(" Resetting WDT...");
            esp_task_wdt_reset();
            delay(1);  // Очень важно для применения сброса сторожевого пса установить задержку минимум 1 миллисекунда [4](https://f1atb.fr/forum_f1atb/thread-174.html)
        if (i % 20 == 10) {  // Нет сброса после 10 секунд для теста
            Serial.println("Stopping WDT reset.");
        }
    }
}
*/

#include <esp_task_wdt.h>
#define WDT_TIMEOUT 25 // Тайм-аут в секундах
esp_err_t ESP32_ERROR;
int i = 0;
int last = millis();

// ****************************************************************************
// * ---    Вывести трассировочное сообщение в com-порт и в 0 строку дисплея     *
// ****************************************************************************
void setup() 
{
  Serial.begin(115200);
  delay(100);
  Serial.println("Configuring WDT...");
  Serial.print("Watchdog Timeout (in seconds) set to : ");
  Serial.println(WDT_TIMEOUT);
  esp_task_wdt_deinit();
  // Task Watchdog configuration - Конфигурация таймера сторожевого пса
  esp_task_wdt_config_t wdt_config = 
  {
    .timeout_ms = WDT_TIMEOUT * 1000,                 // Конвертация в мс
    .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,  // Bitmask of all cores - Битовая маска всех ядер
    .trigger_panic = true                             // Enable panic to restart ESP32 - Включить панику для перезагрузки ESP32
  };
  //
   ESP32_ERROR = esp_task_wdt_init(&wdt_config);
   Serial.println("Last Reset : " + String(esp_err_to_name(ESP32_ERROR)));
   esp_task_wdt_add(NULL);  //add current thread to WDT watch- добавить текущую нить в наблюдение за TWDT
}

// ****************************************************************************
// *  ---   Вывести трассировочное сообщение в com-порт и в 0 строку дисплея     *
// ****************************************************************************


void loop() 
{
  if (millis() - last >= 1000) 
  { // Task every second
    last = millis();
    Serial.print(i);
    Serial.print("s : ");
    i++;
    if (i % 20 <= 10 && i<60) 
      { //Reset only during 10s the first minute
        Serial.println(" Resetting WDT...");
        esp_task_wdt_reset();
        delay(1);  //VERY VERY IMPORTANT for Watchdog Reset to apply. At least 1 ms
        if (i % 20 == 10) 
        { // No reset after 10s for test
          Serial.println("Stopping WDT reset.");
        }
     }
  }
}


// *************************************************************** mk32.ino ***


