// QueueHandlMulti2.ino

char buffer[1024];
struct AMessage
{
   int  ucSize;
   char ucData[256];
} xMessage;
struct AMessage *pxMessage;

unsigned long nLoop = 0UL;

QueueHandle_t xQueue;

void setup() 
{
  Serial.begin(115200);
  
  // Создаем очередь, способную содержать 10 структур, которые должны
  // быть помещены в очередь с помощью указателей, поскольку представляют
  // собой относительно большие данные
  xQueue = xQueueCreate(10, sizeof(struct AMessage));
  if(xQueue==NULL)
  {
    Serial.println("Очередь не была создана и не может использоваться!");
  }
  Serial.println("Очередь сформирована!");
}

#define str "str"
#define chr "chr"
String types(String a) {return str;}
String types(char *a)  {return chr;}

 
void loop() 
{

   String temp = "cat123456 Всем привет!";
   char tab2[1024];
   strcpy(tab2, temp.c_str());
   String temp1=String(tab2);
  
   Serial.println(temp);
   Serial.println(types(temp));
   
   Serial.println(tab2);
   Serial.println(types(tab2));

   Serial.println(temp1);
   Serial.println(types(temp1));
   
   Serial.println(temp1.length());
   Serial.println("----");
   
 



  
  nLoop++;
  Serial.print("Цикл: ");
  Serial.println(nLoop);

  if (xQueue != 0)
  {
    // Отправляем указатель на структуру AMessage 
    sprintf(buffer, "Передан номер цикла: (%d)", nLoop);
    xMessage.ucSize = 0;
    while (buffer[xMessage.ucSize]>0) 
    {
       xMessage.ucData[xMessage.ucSize]=buffer[xMessage.ucSize];
       xMessage.ucSize++;
    }
    pxMessage = &xMessage;
    if (xQueueSend(xQueue,pxMessage,10) != pdPASS)
    {
      Serial.println("Не удалось отправить структуру даже после 10 тиков!");
    }
    else 
    {
      Serial.print ("Передано: ");
      Serial.print (xMessage.ucSize);
      Serial.print ("=>");
      Serial.println (xMessage.ucData);
    }
  }
  else 
  {
    Serial.println("Очередь для структур не создана!");
  }

  struct AMessage xRxedStructure;
  if (xQueue != NULL)
  {
    // Получаем сообщение из созданной очереди для хранения сложного
    // структурного сообщения. Блокировка на 10 тиков, если сообщение
    // недоступно немедленно. Значение считывается в структурную
    // переменную AMessage, поэтому после вызова xQueueReceive()  
    // xRxedStructure будет содержать копию сообщения
      
    if (xQueueReceive(xQueue,&xRxedStructure,10) != pdPASS)
    {
      Serial.println("Не удалось принять структуру даже после 10 тиков!");
    }
    else
    {
      Serial.print("Выбрано: ");
      Serial.println(xRxedStructure.ucSize);
      Serial.println(xRxedStructure.ucData);
      /*
      //Serial.println(String(xRxedStructure.ucData));
      Serial.print(xRxedStructure.ucData[0]);
      Serial.print(xRxedStructure.ucData[1]);
      Serial.print(xRxedStructure.ucData[2]);
      Serial.print(xRxedStructure.ucData[3]);
      Serial.print(xRxedStructure.ucData[4]);
      Serial.print(xRxedStructure.ucData[5]);
      Serial.println("---------------------");
      */
    }
  }
  else
  {
    Serial.println("Нет очереди!");
  }
  delay(1000);
}


void vATask(void *pvParameters)
{
}
