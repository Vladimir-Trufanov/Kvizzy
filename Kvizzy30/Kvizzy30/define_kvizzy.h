/** Arduino, Esp32-CAM ************************************ define_kvizzy.h ***
 * 
 * Определить переменные и константы нижнего уровня умного хозяйства на двух светодиодах
 * 
 * v3.3.2, 23.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>

// Значения чисел, считанных из последовательного порта, 
// иммитирующие ошибочные ситуации 
#define loopingLed33     1     // зацикливание задачи vLed33
#define loopingState     2     // зацикливание задачи vState
#define loopingCore1     3     // зацикливание задачи vCore1
#define loopingCore0     4     // зацикливание задачи vCore0
#define disaflashing33  51     // отключение мигания контрольного светодиода
#define enabflashing33  52     // включение мигания контрольного светодиода

// Управление прерываниями, задачами, деятельностями

// Управление светодиодами
#define PinLedWork      33            // контакт рабочего светодиода
volatile int lastLedWork = millis();  // прошлое время смены состояния контрольного светодиода
volatile int millLedWork = 3017;      // текущий интервал смены состояния контрольного светодиода (чуть более 3 секунд)
#define PinLedFlash      4            // контакт светодиода-вспышки

// Назначаем задачи и флаги сторожевого таймера
void vLed33(void *pvParameters);
bool fwdtLed33 = false;
void vState(void *pvParameters);
bool fwdtState = false;
void vCore0(void *pvParameters);
bool fwdtCore0 = false;
void vCore1(void *pvParameters);
bool fwdtCore1 = false;

// Сбрасываем флаг основного цикла
bool fwdtLoop = false;

// Определяем число, которое будет считываться в основном цикле
// с последовательного порта - команду инициирования зависания процессора
volatile int iCreateSit;
// Определяем временную метку сообщения о зависании процессора
volatile int mitMimic=millis();
// Имитируем событие зависания процессора
void MimicMCUhangEvent(String NameTask)
{
   while (true)
   {
      int mitMimici=millis();
      if ((mitMimici-mitMimic)>1005)
      {
         Serial.print(NameTask);
         Serial.println(": зависание процессора!!!");
         mitMimic = mitMimici;
      }
   }
}

// ******************************************************** define_kvizzy.h ***
