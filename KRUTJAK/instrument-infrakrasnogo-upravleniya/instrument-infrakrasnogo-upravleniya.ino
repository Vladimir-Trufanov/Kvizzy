/** Arduino UNO                *** instrument-infrakrasnogo-upravleniya.ino ***
 * 
 * Построить модель администратора мотора - инструмента инфракрасного управления
 * 
 * v1.1, 02.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.03.2024
**/

#include <IRremote.h>
#include <iarduino_VCC.h> 

#include "define_kru.h"   // подключили определения системы управления
#include "Motor_kru.h"    // подключили драйвер мотора
#include "Admin_kru.h"    // подключили драйвер мотора
#include "Irq_kru.h"      // подключили администратор мотора

// Инициируем драйвер мотора
MotorKrutjak Motor(PinPWM_L,PinPWM_R,PinRes); 
// Инициируем aдминистратор мотора
AdminKrutjak Admin; 
// Инициируем структуру состояния мотора 
Condition Condition_Motor;

void setup() 
{
   Serial.begin(9600);
   pinMode(LEDPIN,OUTPUT);
   cntr=0;

   //IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

   // Инициируем начальное состояние светодиода - "не горит"
   doBurns=false;
   // Настраиваем прерывание по переполнению 2 таймера 
   // для сигнализирования трассировок 
   TrassInit();  
   // Отсоединяем мотор
   Motor.Disconnect();
   
   //Admin.Init(); 
}

void loop() 
{
   // !!! Здесь, в основном цикле программные задержки используются только при
   // отработке команды инфракрасного приемника = 0.2 сек

   delay(1000);
   Motor.Driver(600);
   
   // Проверяем флаг истечения 1 секунды, если флаг установлен (true), то
   // выполняем трассировку и передаём информацию внешнему контроллеру
   if (OneSecondFlag == true)
   {
      // Запускаем цикл моторного теста
      // Motor.Test();
      // Меняя состояние, мигаем светодиодом
      doBurns=!doBurns; 
      digitalWrite(LEDPIN,doBurns);
      // Вытаскиваем состояние драйвера мотора
      Condition_Motor = Motor.Take();
      // Передаём данные внешнему, контроллирующему устройству
      // (выводим данные в последовательный порт)
      // ------------------------------------------------------ Напряжение питания
      Serial.print("Vcc="); Serial.print(analogRead_VCC()); Serial.print("V\r\n");
      // ------------------------------------------------------ Состояние мотора
      Serial.print(Condition_Motor.ValRes);   Serial.print(": ");
      Serial.print(Condition_Motor.ValPWM_L); Serial.print(" - ");
      Serial.println(Condition_Motor.ValPWM_R);
      // Cбрасываем флаг одной секунды
      OneSecondFlag=false;            
   }
   
   /*
   if (IrReceiver.decode()) 
   {
      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      uint16_t command = IrReceiver.decodedIRData.command;
      //Serial.println("command");
      delay(100);  
      
      Admin.Steward(Motor,command); 
      
      // Делаем задержку в 100 мс, чтобы замедлить печать при непрерывном 
      // нажатии клавиши, и устранить повторную распечатку
      delay(100);  
      // Готовим прием следующего нажатия клавиши
      IrReceiver.resume();
   }
   */
}

// ******************************* instrument-infrakrasnogo-upravleniya.ino ***
