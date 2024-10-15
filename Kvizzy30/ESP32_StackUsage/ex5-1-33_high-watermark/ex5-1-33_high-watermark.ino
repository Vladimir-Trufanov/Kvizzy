/** Arduino-Esp32-CAM                       *** ex5-1-33_high-watermark.ino ***

                        Вычислить объем памяти (стека), требующийся для задачи. 
   Исследуемые задачи просто мигают контрольным светодиодом и вспышкой на своих 
   штатных контактах 4 и 33 (для ESP32-CAM). При объявлении задач резервируется 
             достаточный объем памяти (но различный для каждой задачи для пущей 
    убедительности), а затем используется функция uxTaskGetStackHighWaterMark() 
           для запроса оставшейся свободной памяти в зарезервированной области.    

     Первоисточник: 
     https://wolles-elektronikkiste.de/en/using-freertos-with-esp32-and-arduino

   v1.0, 13.10.2024                                   Автор:      Труфанов В.Е.
   Copyright © 2024 tve                               Дата создания: 13.10.2024

**/

#define LED1 4  // определили пин вспышки
#define LED2 33 // определили пин контрольного светодиода

// Определяем дескрипторы задач
TaskHandle_t taskBlink1Handle; 
TaskHandle_t taskBlink2Handle;

void setup()
{
   Serial.begin(115200);
   pinMode(LED1, OUTPUT);
   pinMode(LED2, OUTPUT);

   xTaskCreate(
      blink1,             // Function name of the task
      "Blink 1",          // Name of the task (e.g. for debugging)

   // 1048,               // Достаточный размер стека (bytes)
      500,                // Недостаточный размер стека, который вызывает переполнение стека и перезапуск контроллера:
                          //    Guru Meditation Error: Core  0 panic'ed (Unhandled debug exception). 
                          //    Debug exception reason: Stack canary watchpoint triggered (Blink 1) 

      NULL,               // Parameter to pass
      1,                  // Task priority
      &taskBlink1Handle   // Assign task handle
   );

   xTaskCreate(
      blink2,             // Function name of the task
      "Blink 2",          // Name of the task (e.g. for debugging)
      2048,               // Stack size (bytes)
      NULL,               // Parameter to pass
      1,                  // Task priority
      &taskBlink2Handle   // Assign task handle
   );

   xTaskCreate(
      printWatermark,     // Function name of the task
      "Print Watermark",  // Name of the task (e.g. for debugging)
      2048,               // Рекомендуемый размер стека от Espressif Systems (в байтах)
      NULL,               // Parameter to pass
      1,                  // Task priority
      NULL                // Task handle
   );
}

void blink1(void *pvParameters)
{
   while (1)
   {
      digitalWrite(LED1, HIGH);
      delay(100);
      digitalWrite(LED1, LOW);
      delay(1900);
   }
}

void blink2(void *pvParameters)
{
   while (1)
   {
      digitalWrite(LED2, HIGH);
      delay(333);
      digitalWrite(LED2, LOW);
      delay(333);
   }
}

void printWatermark(void *pvParameters)
{
   while (1)
   {
      delay(2000);
      Serial.print("TASK: ");
      Serial.print(pcTaskGetName(taskBlink1Handle)); // Get task name with handler
      Serial.print(", High Watermark: ");
      Serial.print(uxTaskGetStackHighWaterMark(taskBlink1Handle));
      Serial.println();
      Serial.print("TASK: ");
      Serial.print(pcTaskGetName(taskBlink2Handle)); // Get task name with handler
      Serial.print(", High Watermark: ");
      Serial.print(uxTaskGetStackHighWaterMark(taskBlink2Handle));
      Serial.println();
   }
}

void loop() {}

// ******************************************** ex5-1-33_high-watermark.ino ***
