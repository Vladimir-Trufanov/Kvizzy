/** ittve-ESP32-CAM                                     *** tveEsp32Cam10.h ***
 * 
 * Библиотека приватных функций ESP32-CAM 
 * 
 * v1.0, 20.07.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 20.07.2024
**/

#ifndef tveEsp32Cam
#define tveEsp32Cam
#pragma once            

// Пины для мигания лампочек
#define LedWorkEsp32Cam  33     // контакт рабочего светодиода
#define LedFlashEsp32Cam  4     // контакт светодиода-вспышки

// Режимы мигания лампочек
#define ModeBlink        20     // режим для текущего приложения

#define MainBlink        10     // поочередное мигание светодиодов
#define WorkBlink        20     // мигание рабочей лампочки
#define FlashBlink       30     // мигание вспышки

// ****************************************************************************
// *              Перевести контакты лампочек в режим вывода                  *
// ****************************************************************************
void ini32CamBlinks() 
{
   // Переводим контакты лампочек в режим вывода
   #if ModeBlink >= FlashBlink
      pinMode(LedFlashEsp32Cam,OUTPUT);   // "вспышка"
   #elif ModeBlink >= WorkBlink 
      pinMode(LedWorkEsp32Cam,OUTPUT);    // "работает"
   #else
      pinMode(LedWorkEsp32Cam,OUTPUT);    // "работает"
      pinMode(LedFlashEsp32Cam,OUTPUT);   // "вспышка"
   #endif
}

// ****************************************************************************
// *                     Выполнить один цикл мигания лампочек                 *
// ****************************************************************************
void loop32CamBlinks(word DelayTicks=500) 
{
   #if ModeBlink == MainBlink
      digitalWrite(LedWorkEsp32Cam,LOW);
      digitalWrite(LedFlashEsp32Cam,LOW);
      delay(DelayTicks);
      digitalWrite(LedWorkEsp32Cam,HIGH);
      digitalWrite(LedFlashEsp32Cam,HIGH);
      delay(DelayTicks);
   #elif ModeBlink == WorkBlink 
      digitalWrite(LedWorkEsp32Cam,LOW);
      delay(DelayTicks);
      digitalWrite(LedWorkEsp32Cam,HIGH);
      delay(DelayTicks);
   #elif ModeBlink == FlashBlink 
      digitalWrite(LedFlashEsp32Cam,LOW);
      delay(DelayTicks);
      digitalWrite(LedFlashEsp32Cam,HIGH);
      delay(DelayTicks);
   #else
      digitalWrite(LedWorkEsp32Cam,LOW);
      delay(100);
      digitalWrite(LedWorkEsp32Cam,HIGH);
      delay(100);
   #endif
}

#endif

// ******************************************************** tveEsp32Cam10.h ***
