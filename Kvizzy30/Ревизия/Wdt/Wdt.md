## [Wdt](#)

#### "как настроить сторожевой таймер esp32 в ардуино"

### Библиография

#### [ESP32 Watchdog example](https://f1atb.fr/esp32-watchdog-example/)

Сторожевой таймер ESP32, начиная с версии управления платой 3.0x, в значительной степени изменен по сравнению с предыдущей реализацией. Вот простой пример того, как это реализовать (Arduino IDE).
Ключевым моментом является установка задержки не менее 1 мс после каждого сброса:
```
esp_task_wdt_reset();
задержка(1); // <---------- Обязательна
```


```
#include <esp_task_wdt.h>
#define WDT_TIMEOUT 25 // Timeout in seconds
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
  // Task Watchdog configuration
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = WDT_TIMEOUT * 1000,                 // Convertin ms
    .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,  // Bitmask of all cores, https://github.com/espressif/esp-idf/blob/v5.2.2/examples/system/task_watchdog/main/task_watchdog_example_main.c
    .trigger_panic = true                             // Enable panic to restart ESP32
  };
  // WDT Init
  ESP32_ERROR = esp_task_wdt_init(&wdt_config);
   Serial.println("Last Reset : " + String(esp_err_to_name(ESP32_ERROR)));
   esp_task_wdt_add(NULL);  //add current thread to WDT watch
}

void loop() {
  if (millis() - last >= 1000) { // Task every second
    last = millis();
    Serial.print(i);
    Serial.print("s : ");
    i++;
    if (i % 20 <= 10 && i<60) { //Reset only during 10s the first minute
      Serial.println(" Resetting WDT...");
      esp_task_wdt_reset();
      delay(1);  //VERY VERY IMPORTANT for Watchdog Reset to apply. At least 1 ms
      if (i % 20 == 10) { // No reset after 10s for test
        Serial.println("Stopping WDT reset.");
      }
    }
  }
}
```


#### [Http request ESP32 Tutorial](https://esp32io.com/tutorials/esp32-http-request)


###### [в начало](#%D0%B2%D1%81%D0%B5-%D0%BF%D1%80%D0%BE-%D0%BF%D0%B0%D0%BC%D1%8F%D1%82%D1%8C-esp32)
