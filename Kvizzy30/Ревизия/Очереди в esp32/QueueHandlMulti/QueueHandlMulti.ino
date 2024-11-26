// QueueHandlMulti.ino

struct AMessage
{
   char ucMessageID;
   char ucData[20];
} xMessage;
 
unsigned long ulVar = 10UL;

void vATask0(void *pvParameters)
{
   QueueHandle_t xQueue1, xQueue2;
   // Создаём очередь, способную содержать до 10 чисел типа unsigned long
   xQueue1 = xQueueCreate(10, sizeof(unsigned long));
   if(xQueue1==NULL)
   {
      /* Очередь не была создана и не должна использоваться */
   }
   // Создаем очередь, способную содержать 10 структур, которые должны
   // быть помещены в очередь с помощью указателей, поскольку представляют
   // собой относительно большие данные
   xQueue2 = xQueueCreate(10, sizeof(struct AMessage));
   if( xQueue2 == NULL )
   {
      /* Очередь не была создана и не должна использоваться */
   }
   /* ... остальная часть кода задачи */
}

void vATask1(void *pvParameters)
{
   QueueHandle_t xQueue1, xQueue2;
   struct AMessage *pxMessage;
   
   // Создаём очередь, способную содержать 10 длинных целых без знака
   xQueue1 = xQueueCreate(10, sizeof(unsigned long));
   // Создаём очередь, способную содержать 10 указателей на структуры
   // сообщений. Они должны передаваться по указателю, поскольку содержат
   // длинные строки
   xQueue2 = xQueueCreate(10, sizeof(struct AMessage *));
   
   /* ... */
   
   if(xQueue1 != 0)
   {
      // Посылаем беззнаковые целые. Ждем 10 секунд, пока освободится 
      // место, при необходимости освобождаем его
      if (xQueueSend(xQueue1, (void *) &ulVar, (TickType_t) 10) != pdPASS)
      {
         /* Не удалось отправить сообщение даже после 10 тиков */
      }
   }
   
   if (xQueue2 != 0)
   {
      // Отправляем указатель на структуру AMessage (не блокируем, если
      // очередь уже заполнена
      pxMessage = & xMessage;
      xQueueSend(xQueue2, ( void *) &pxMessage, (TickType_t) 0);
   }
   /* ... остальная часть кода задачи */
}

/*
// Определяем переменную-структуру AMessage
struct AMessage
{
   char ucMessageID;
   char ucData[20];
}  xMessage;

// Определяем очередь, которая будет использоваться для отправки 
// и получения сообщений в виде структуры типа AMessage
QueueHandle_t xStructQueue = NULL;

// Определяем очередь, которая будет использоваться для отправки
// и получения указателей на структуры
QueueHandle_t xPointerQueue = NULL;

void vCreateQueues(void) 
{
   xMessage.ucMessageID = 0xab;
   memset(&(xMessage.ucData), 0x12, 20);
   
   // Создаём очередь для отправки полных сообщений-структур. Ее также
   // можно создать и позже, но необходимо позаботиться о том, чтобы
   // очередь не использовалась до тех пор, пока она не будет создана
   xStructQueue  =  xQueueCreate(10, sizeof(xMessage));
   
   // Создаём очередь для отправки указателей на структуры AMessage
   xPointerQueue = xQueueCreate(10, sizeof(&xMessage));
   
   if ((xStructQueue == NULL) || (xPointerQueue == NULL))
   {
      // "Не удалось создать одну или несколько очередей из-за нехватки
      // доступной памяти в куче. Исправьте ошибку здесь. 
      // Очереди также могут быть созданы  статически
   }
}

// Задача, которая выполняет запись в очередь
void vATask(void *pvParameters)
{
   struct AMessage *pxPointerToxMessage;
   
   // Отправляем всю структуру в очередь, созданную для хранения 10 структур
   xQueueSend( 
      // Дескриптор очереди
      xStructQueue,
      // Адрес переменной xMessage  sizeof(struct AMessage), откуда
      // байты копируются в очередь
      (void *) &xMessage,
      // Время блокировки, равное 0, означающее, что не нужно блокировать 
      // задачу, если очередь уже заполнена. Следует проверять значение,
      // возвращаемое функцией xQueueSend(), чтобы узнать, было ли
      // сообщение успешно отправлено в очередь
      (TickType_t  0)
   );
                             
   // Сохраняем адрес переменной xMessage в переменной-указателе
   pxPointerToxMessage = &xMessage;
   
   // Отправляем адрес сообщения в очередь, созданную 
   // для хранения 10 указателей
   xQueueSend (xPointerQueue, (void *) &pxPointerToxMessage,  (TickType_t) 0);
                
   / * ... некоторая часть кода * /
}

// Задача, которая выполняет чтение из очереди
void vADifferentTask(void *pvParameters)
{
   struct AMessage xRxedStructure, *pxRxedPointer;
   if (xStructQueue != NULL)
   {
      // Получаем сообщение из созданной очереди для хранения сложного
      // структурного сообщения. Блокировка на 10 тиков, если сообщение
      // недоступно немедленно. Значение считывается в структурную
      // переменную AMessage, поэтому после вызова xQueueReceive()  
      // xRxedStructure будет содержать копию сообщения
      
      if (xQueueReceive(xStructQueue, &(xRxedStructure), (TickType_t) 10) == pdPASS)
      {
         / * xRxedStructure теперь содержит копию xMessage. * /
      }
   }
   
   if (xPointerQueue != NULL)
   {
      // Получаем сообщение из созданной очереди для хранения указателей.
      // Блокируем на 10 тиков, если сообщение недоступно - немедленно.
      // Значение считывается в переменную указателя, и полученное значение
      // является адресом сообщения. Переменная, после этого вызова
      // pxRxedPointer будет указывать на xMessage.
      
      if (xQueueReceive(xPointerQueue,  &(pxRxedPointer), (TickType_t) 10) == pdPASS)
      {
         / * *pxRxedPointer теперь указывает на xMessage. * /
      }
   }
   / * ... остальная часть кода задачи * /
}  
*/

void setup() 
{
}

void loop() 
{
}


/*
QueueHandle_t queue;
 
void setup() 
{
  Serial.begin(115200);
  queue = xQueueCreate( 10, sizeof( int ) );
  if(queue == NULL)
  {
    Serial.println("Error creating the queue");
  }
}
 
void loop() 
{
  if(queue == NULL)return;
  for(int i = 0; i<10; i++)
  {
    xQueueSend(queue, &i, portMAX_DELAY);
  }
  
  int element;
  for(int i = 0; i<10; i++)
  {
    xQueueReceive(queue, &element, portMAX_DELAY);
    Serial.print(element);
    Serial.print("|");
  }
  Serial.println();
  delay(1000);
}
*/
