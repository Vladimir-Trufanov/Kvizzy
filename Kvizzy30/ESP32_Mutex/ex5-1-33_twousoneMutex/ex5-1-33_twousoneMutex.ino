/** Arduino-Esp32-CAM                        *** ex5-1-33_twousoneMutex.ino ***
 *  
 *          Пример работы трех задач в ESP32 board 5.1.33: основной цикл мигает
 *             контрольным светодиодом, а две другие задачи делят между собой с
 *  помощью мьютекса счетчик и последовательный порт, на который выводят тексты 
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 * 
 * v1.0, 13.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 13.10.2024
**/

// Определяем пин контрольного светодиода
#define LED_BUILTIN 33
// Определяем объект мьютекса - дескриптор (во FreeRTOS и мьютекс, и семафор реализованы
// как обычные совместно используемые подпрограммы. Это связано со сходством между обеими 
// концепциями. Дескриптор семафора может использоваться для создания семафоров любого 
// типа или мьютекса. xMutex это только дескриптор, инициализированный NULL значением. 
// Пока его невозможно использовать, это будет сделано в setup() 
// с помощью xSemaphoreCreateMutex())
SemaphoreHandle_t xMutex = NULL;  
// Определяем глобальную переменную counter, которая будет действовать как общий ресурс. 
// Две задачи - task1 и task2 могут обращаться к переменной counter. Однако, поскольку 
// это общий ресурс, задачи выполняются параллельно, нужен мьютекс для предотвращения 
// конфликтов
int counter = 0;  // A shared variable

// ============================================================================
void setup() 
{
   pinMode (LED_BUILTIN, OUTPUT);
   Serial.begin (115200);
   // Создаем объект мьютекса - мьютекс
   xMutex = xSemaphoreCreateMutex();  
   // Cоздаем задачи, привязывая их к ядру 0
   xTaskCreatePinnedToCore 
   (
      task1,     // Function to implement the task
      "task1",   // Name of the task
      1000,      // Stack size in words
      NULL,      // Task input parameter
      10,        // Priority of the task
      NULL,      // Task handle.
      0          // Core where the task should run
   );

   xTaskCreatePinnedToCore 
   (
      task2,     // Function to implement the task
      "task2",   // Name of the task
      1000,      // Stack size in words
      NULL,      // Task input parameter
      10,        // Priority of the task
      NULL,      // Task handle.
      0          // Core where the task should run
   );
}

// ============================================================================ 
void loop() 
{
  digitalWrite (LED_BUILTIN, HIGH);  
  delay (1000);                      
  digitalWrite (LED_BUILTIN, LOW);   
  delay (1000);                      
}

// ============================================================================
// Сначала в задаче task1 пытаемся захватить и заблокировать мьютекс xMutex с помощью функции 
// xSemaphoreTake(xMutex,portMAX_DELAY). Функции передаются два параметра: дескриптор мьютекса 
// и значение таймаута. Здесь используется portMAX_DELAY макрос, который соответствует 
// неопределенной задержке. Это означает, что task1 будет пытаться получить блокировку 
// на неопределенный срок, пока не добьется успеха. Когда получаем блокировку, 
// то печатаем некоторую информацию и увеличиваем значение counter на 1.
// Затем печатаем значение счетчика и ждем 1 секунду, прежде чем снять 
// блокировку мьютекса с помощью вызова xSemaphoreGive(xMutex), отдаем мьютекс и ждем еще 800 
// миллисекунд, прежде чем повторить всю операцию.
void task1 (void *pvParameters) 
{
  while (1) 
  {
    // Как только захватили мьютекс, выполняем свою работу
    if (xSemaphoreTake (xMutex, portMAX_DELAY)) 
    {  
      Serial.print ("Task 1: Mutex взят задачей ");
      Serial.println (xTaskGetTickCount());
      counter = counter + 1;  
      Serial.print ("Task 1: Counter = ");
      Serial.println (counter);
      delay (1000);
      xSemaphoreGive (xMutex);  
      delay (800);
    }
  }
}

// ============================================================================
// В task2 конкурируем за ту же counter переменную. Пытаемся заблокировать xMutex 
// с таймаутом в 200 миллисекунд, указанным как 200*portTICK_PERIOD_MS. Если захватить
// блокировку не получается в течение этого периода времени, прекращаем попытки 
// и вместо этого печатает сообщение. 
//
// !!! Блокировка task2 может быть установлена, только если она находится в состоянии 
// разблокировки или, другими словами, не заблокирована task1. Поскольку для освобождения 
// мьютекса task1 требуется 1 секунда, task2 придется подождать не менее 1 секунды, 
// прежде чем он сможет получить блокировку. 
//
// Это будет видно в выводе на последовательный монитор
void task2 (void *pvParameters) 
{
   while (1) 
   {
      if (xSemaphoreTake (xMutex, (200 * portTICK_PERIOD_MS))) 
      { 
         Serial.print ("Task 2: Mutex взят задачей ");
         Serial.println (xTaskGetTickCount());
         counter = counter + 1000;
         Serial.print ("Task 2: Counter = ");
         Serial.println (counter);
         xSemaphoreGive (xMutex);  
         delay (1200);
      }
      else 
      {  
         Serial.print ("Task 2: Mutex не захвачен ");
         Serial.println (xTaskGetTickCount());
      }
  }
}

// ********************************************* ex5-1-33_twousoneMutex.ino ***
