/** Arduino-Esp32-CAM                    *** ex5-1-33_twousoneSemaphore.ino ***
 *  
 *         Пример работы с семафором в ESP32 board 5.1.33: основной цикл мигает
 *    контрольным светодиодом. Задача №1 вертится в своем цикле и раз в секунду
 *     отпускает семафор, то есть подает сигнал другим задачам, чтобы они могли 
 *     учесть семафор и выполнить свою работу. Задача №2 также крутится в своем 
 *    цикле, ждет освобождения семафора, при освобождении захватывает семафор и
 * выполняет свою работу. П1 pflfxf а две другие задачи делят между собой с
 *  помощью мьютекса счетчик и последовательный порт, на который выводят тексты 
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 * 
 * v1.0, 13.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 13.10.2024
**/

// Определяем пин контрольного светодиода
#define LED_BUILTIN 33


//=============================================================================
// the setup function runs once when you press reset or power the board
void setup() 
{
   pinMode (LED_BUILTIN, OUTPUT);
   Serial.begin (115200);
   // Определяем объект семафора - дескриптор
   SemaphoreHandle_t xSemaphore = NULL;  
   // Создаем объект двоичного семафора
   xSemaphore = xSemaphoreCreateBinary();  
   // Создаем две задачи на нулевом ядре
   xTaskCreatePinnedToCore (
      task1,     // Function to implement the task
      "task1",   // Name of the task
      1000,      // Stack size in words
      NULL,      // Task input parameter
      10,        // Priority of the task
      NULL,      // Task handle.
      0          // Core where the task should run
   );
   xTaskCreatePinnedToCore (
      task2,     // Function to implement the task
      "2000",    // Name of the task
      1000,      // Stack size in words
      NULL,      // Task input parameter
      10,        // Priority of the task
      NULL,      // Task handle.
      0          // Core where the task should run
   );
}

//=============================================================================
// the loop function runs over and over again forever
void loop() 
{
   digitalWrite (LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
   delay (1000);                      // wait for a second
   digitalWrite (LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
   delay (1000);                      // wait for a second
}

//=============================================================================
// this task will periodically release the binary semaphore
void task1 (void *pvParameters) 
{
   while (1) 
   {
      // Каждую секунду отпускаем семафор
      Serial.print ("task1: Binary Semaphore released at ");
      Serial.println (xTaskGetTickCount());
      xSemaphoreGive (xSemaphore);    
      delay (1000);
   }
}

//=============================================================================
// this task will wait for the binary semaphore to be released
void task2 (void *pvParameters) 
{
   while (1) 
   {
      if (xSemaphoreTake (xSemaphore, (200 * portTICK_PERIOD_MS))) 
      {  // try to acquire the semaphore
         Serial.print ("task2: Binary Semaphore acquired at ");
         Serial.println (xTaskGetTickCount());
      }
      else 
      {  // if the semaphore was not acquired within 200ms
         Serial.print ("task2: Binary Semaphore not acquired at ");
         Serial.println (xTaskGetTickCount());
      }
   }
}

//=============================================================================

// ***************************************** ex5-1-33_twousoneSemaphore.ino ***
