/** Arduino, Esp32-CAM ***************************************** kviDHT11.h ***
 * 
 *   Обеспечить взаимодействие датчика DHT11 c контроллером ESP32-CAM по GPIO12 
 *          и передачу данных о температуре и влажности на страницу сайта State
 * 
 * v1.0.0, 20.04.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 20.04.2025
**/

#pragma once            

#include <DHT.h>
#define DHT11_PIN  13 

DHT dht11(DHT11_PIN, DHT11);

// * Задача FreRTOS ***********************************************************
// *                   Обеспечить взаимодействие датчика DHT11 c контроллером *
// *       ESP32-CAM по GPIO12 и передачу данных о температуре и влажности на *
// *                                                   страницу сайта State   *
// ****************************************************************************
void vDHT11(void* pvParameters)
/*
{
  "led4": [
    {
      "nicdev": "led33",
      "tiddev": 1,
      "light": 55,
      "time": 2004,
      "regim": 0,
      "status": "inLOW"
    }
  ]
}
*/
{
  // Активируем датчик температуры и влажности
  dht11.begin(); 
  Serial.println("DHT11 - датчик температуры и влажности активирован");

  for (;;)
  {
    //Serial.println("*** vDHT11 ***");
    
    // Считываем влажность
    float humi  = dht11.readHumidity();
    // Считываем температуру
    float tempC = dht11.readTemperature();
    // Проверяем, успешно ли выполнено считывание
    if (isnan(tempC) || isnan(humi)) 
    {
      Serial.println("Ошибка чтения датчика DHT11!");
    } 
    else 
    {
      /*
      Serial.print("Влажность: ");
      Serial.print(humi);
      Serial.print("%");

      Serial.print("  |  ");

      Serial.print("Tемпература: ");
      Serial.print(tempC);
      Serial.println("°C");
      */
    }

    // Отмечаем флагом, что цикл задачи успешно завершен   
    fwdtDHT11 = true;
    // Ничего не делаем 2 секунды
    vTaskDelay(2123/portTICK_PERIOD_MS); 
  }
}

// ************************************************************* kviDHT11.h ***
