/** Arduino-Esp32-CAM                 *** ex5-1-33_twousonePortCritical.ino ***
 *  
 *                 Пример использования критических секций в ESP32 board 5.1.33
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 *                                        
 *        Ход исполнения любых последовательных или параллельных задач, которые 
 *            крутятся в контроллере всегда может быть остановлен для обработки
 *     прерывания. После обработки прерывания ситуация внутри прерванной задачи 
 *  может измениться, что повлечет за собой неправильное исполнение дальнейшего 
 *      кода в задаче. Для решения этой проблемы, FreeRTOS позволяет определять 
 *     критически важные фрагменты кода с помощью макросов taskENTER_CRITICAL() 
 *     и taskEXIT_CRITICAL(), которые все-таки не прерываются при возникновении 
 *                                          прерываний и выполняются полностью.   
 *                                        
 * https://circuitstate.com/tutorials/how-to-write-parallel-multitasking-applications-for-esp32-with-freertos-arduino
 * 
 * v1.0, 16.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 16.10.2024
**/

// Определяем пин контрольного светодиода
#define LED_BUILTIN 33
// Определяем мьютекс, который будет связан с критической секцией
// и проинициализируем его (то есть разблокируем для дальнейшего захвата)
portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED; 
// Создаем контроллируемую переменную критической секции
int counter = 0; 

//=============================================================================
// the setup function runs once when you press reset or power the board
void setup() 
{
   // initialize digital pin LED_BUILTIN as an output.
   pinMode (LED_BUILTIN, OUTPUT);
   Serial.begin (115200);
   xTaskCreatePinnedToCore (
      task1,     // Function to implement the task
      "task1",   // Name of the task
      1024,      // Stack size in words
      NULL,      // Task input parameter
      10,        // Priority of the task
      NULL,      // Task handle.
      0          // Core where the task should run
   );
   
   xTaskCreatePinnedToCore (
      task2,     // Function to implement the task
      "2000",    // Name of the task
      1024,      // Stack size in words
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
// this task will periodically lock the mutex, increment the counter by 1 and unlock the mutex
void task1 (void *pvParameters) 
{
   while(1) 
   {
      Serial.print ("Try task1: ");
      portENTER_CRITICAL (&taskMux);  // lock the mutex (busy waiting)
         counter = counter + 1;       // increment the counter
         Serial.print ("Counter = ");
         Serial.println (counter);
      portEXIT_CRITICAL (&taskMux);   // unlock the mutex
      delay (1000);
   }
}

//=============================================================================
// this task will periodically lock the mutex, increment the counter by 1000 and unlock the mutex
void task2 (void *pvParameters) 
{
   while (1) 
   {
      Serial.print ("Try task2: ");
      portENTER_CRITICAL (&taskMux);  // lock the mutex (busy waiting)
         counter = counter + 1000;
         Serial.print ("Counter = ");
         Serial.println (counter);
      portEXIT_CRITICAL (&taskMux);   // unlock the mutex
      delay (500);
   }
}

// ************************************** ex5-1-33_twousonePortCritical.ino ***
