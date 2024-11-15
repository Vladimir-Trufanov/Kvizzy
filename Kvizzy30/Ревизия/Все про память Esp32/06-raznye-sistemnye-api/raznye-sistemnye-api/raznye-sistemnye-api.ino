/** Arduino ESP32-CAM **************************** raznye-sistemnye-api.ino ***
 * 
 * Разобраться и проверить системные функции Espressif ESP32 на ESP32-CAM
 * по статье Miscellaneous System APIs:
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/misc_system_api.html#_CPPv411esp_restartv
 * 
 * v1.1, 22.07.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 20.07.2024
**/

// 2024-07-20
// 
// Payment:                              "Al Thinker ESP32-CAM"
// CPU Frequency:                        "240MHz (WiFi/BT)"
// Flash Frequency:                      "80MHz"
// Flash Mode:                           "QIO"
// Partition Scheme:                     "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
// Core Debug Level:                     "Ничего"
// Erase All Flash Before Sketch Upload: "Enabled"
// Port:                                 "COM5"

// Additional links for the Board Manager: https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json

// Менеджер плат:         esp32 by Espressif Systems 3.0.1 installed
// Платы в данном пакете: ESP32 Dev Board, ESP32-S2 Dev Board, ESP32-S3 Dev Board, ESP32-C3 Dev Board, Arduino Nano ESP32.

#include "tveEsp32Cam10.h"       

void setup() 
{
   Serial.begin(115200);
   ini32CamBlinks(); 
}

void loop() 
{
   Serial.println("inputString");
   loop32CamBlinks(); 
   //delay(1000);
}

// *********************************************** raznye-sistemnye-api.ino ***
