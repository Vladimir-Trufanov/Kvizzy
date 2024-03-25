//                                                        *** Motor_kru.cpp ***
                                                       

#include "Motor_kru.h" // подключаем заголовок обязательно

void MotorKrutjak::Init(int PinPWM_L, int PinPWM_R) 
{
   pinMode(PinPWM_L, OUTPUT); // назначили, как выход, контакт левого плеча 
   pinMode(PinPWM_R, OUTPUT); // назначили, как выход, контакт правого плеча 
}

void MotorKrutjak::Driver(int PinPWM_L, int PinPWM_R, int PinRes) 
{
   int ValRes = 0;     // считанное значение напряжения с аналогового контакта (0-1023)
   int ValPWM_L = 0;   // хранимое значение ШИМ для отправки на затворы левого плеча (0-255)
   int ValPWM_R = 0;   // хранимое значение ШИМ для отправки на затворы правого плеча (0-255)
  
   ValRes = analogRead(PinRes);                  // считали напряжение с потенциометра
   if (ValRes<512)
   {
      ValPWM_L = map(ValRes, 0,512, 255,0);      // расчитали напряжение ШИМ
      ValPWM_R = 0;   
   }
   else
   {
      ValPWM_R = map(ValRes, 512,1023, 0,255);   // расчитали напряжение ШИМ
      ValPWM_L = 0;   
   }
   analogWrite(PinPWM_L,ValPWM_L);               // установили скорость мотора в дискретах
   analogWrite(PinPWM_R,ValPWM_R);               // установили скорость мотора в дискретах
}


/** Arduino UNO                                               *** Motor.cpp ***
 * 
 * Драйвер электромотора на с использованием широтно-импульсной модуляции, 
 * двух n-канальных полевых транзисторов IRFZ24N и двух p-канальных полевых 
 * транзисторов IRF9Z24N.

 * 
 * v1.1, 25.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 24.03.2024
**/

/*
#ifndef Motor_cpp
#define Motor_cpp
*/
/* Параметры, передаваемые драйверу 
const int PinRes   = A0;     // аналоговый вход, к которому подключен потенциометр
const int PinPWM_L = 10;     // цифровой (ШИМ) выход, к которому подключено левое плечо транзисторов
const int PinPWM_R = 9;      // цифровой (ШИМ) выход, к которому подключено правое плечо
*/

/*
#include "Arduino.h"

void MotoriInit(int PinPWM_L, int PinPWM_R) 
{
   pinMode(PinPWM_L, OUTPUT); // назначили, как выход, контакт 10 Arduino 
   pinMode(PinPWM_R, OUTPUT); 
}

void MotoriDriver(int PinPWM_L, int PinPWM_R, int PinRes) 
{
   int ValRes = 0;     // считанное значение напряжения с аналогового контакта (0-1023)
   int ValPWM_L = 0;   // хранимое значение ШИМ для отправки на затворы левого плеча (0-255)
   int ValPWM_R = 0;   // хранимое значение ШИМ для отправки на затворы правого плеча (0-255)
  
   ValRes = analogRead(PinRes);           // считали напряжение с потенциометра
   if (ValRes<512)
   {
      ValPWM_L = map(ValRes, 0,512, 255,0);   // расчитали напряжение ШИМ
      ValPWM_R = 0;   
   }
   else
   {
      ValPWM_R = map(ValRes, 512,1023, 0,255);   // расчитали напряжение ШИМ
      ValPWM_L = 0;   
   }
   analogWrite(PinPWM_L,ValPWM_L);            // установили скорость мотора в дискретах
   analogWrite(PinPWM_R,ValPWM_R);            // установили скорость мотора в дискретах
}

#endif
*/
// ************************************************************** Motor.cpp ***
