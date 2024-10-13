//#include <Arduino_FreeRTOS.h> 
//#include <semphr.h> 
 
SemaphoreHandle_t mutex_v; 

void setup() 
{ 
    Serial.begin(115200); 
    mutex_v = xSemaphoreCreateMutex(); 
    if (mutex_v == NULL) Serial.println("Mutex создать невозможно"); 
    else 
    {
       Serial.println("Mutex создан!"); 
       xTaskCreate(Task1, "Task1", 128, NULL, 1, NULL); 
       //xTaskCreate(Task2, "Task2", 128, NULL, 1, NULL); 
    }
} 
 
void Task1(void *pvParameters) 
{ 
    while(1) 
    { 
        xSemaphoreTake(mutex_v, portMAX_DELAY); 
        Serial.println("Hi from Task1"); 
        xSemaphoreGive(mutex_v); 
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    } 
} 
 
void Task2(void *pvParameters) 
{ 
    while(1) 
    { 
        xSemaphoreTake(mutex_v, portMAX_DELAY); 
        Serial.println("Hi from Task2"); 
        xSemaphoreGive(mutex_v); 
        vTaskDelay(pdMS_TO_TICKS(500)); 
    } 
} 
 
void loop() 
{ 
}
