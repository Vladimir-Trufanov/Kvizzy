/** Arduino UNO                                               *** Irq_kru.h ***
 * 
 * Подготовить и обработать прерывание от 2-го таймера для трассировки
 * данных и событий с паровозиком "КРУТЯК" с частотой в 1 сек 
 * 
 * v2.3, 01.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 24.03.2024
**/

#ifndef Irq_kru
#define Irq_kru
 
#pragma once            // обеспечили разовое подключение файла
#include <Arduino.h>    // подключили оьщие функции Arduino
#include "Motor_kru.h"  // подключили драйвер мотора

// Состояние таймерного прерывания
struct statusIrq
{
   bool doBurns;  
   int  cntr; 
};
// ****************************************************************************
// *               Настроить прерывание по переполнению 2 таймера             *
// ****************************************************************************
void TrassInit(int cntr, byte nLEDPIN) 
{
   pinMode(nLEDPIN, OUTPUT);
   cntr=0;
   cli(); // отключили прерывания
   // Инициализируем регистры Timer2
   TCCR2A = 0; 
   TCCR2B = 0;
   TIMSK2 = 0;
   // Включаем прерывание Timer2 по переполнению
   TIMSK2 = (1 << TOIE2);
   // Устанавливаем CS10 бит так, чтобы таймер работал при тактовой частоте:
   TCCR2B |= (1 << CS20);
   sei(); // включили прерывания
}
// ****************************************************************************
// *         Контроллировать счетчик прерываний для прошествия 1 сек          *
// *                      и трассировать данные и события                     *
// ****************************************************************************
statusIrq TrassMake(int cntr, unsigned int BtnToggle, bool doBurns, 
   byte nLEDPIN, MotorKrutjak Motor, Condition Condition_Motor) 
{
   statusIrq values;
   if (cntr>BtnToggle)
   {
      // Изменяем состояние переключателя по прошествии 1 секунды
      // и сбрасываем счетчик прерываний
      doBurns=!doBurns;
      cntr=0;
      // Устанавливаем состояние светодиода
      digitalWrite(nLEDPIN,doBurns);
      // Выводим состояние драйвера мотора
      Condition_Motor = Motor.Take();
      Serial.println("---");
      /*
      Serial.print(Condition_Motor.ValRes);   Serial.print(" ");
      Serial.print(Condition_Motor.ValPWM_L); Serial.print(" ");
      Serial.println(Condition_Motor.ValPWM_R);
      */
   }
   values.doBurns = doBurns;  
   values.cntr = cntr; 
   return values;
}

#endif

// ************************************************************** Irq_kru.h ***
