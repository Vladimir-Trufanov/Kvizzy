/** Arduino UNO                                           *** Admin_kru.cpp ***
 * 
 * Администратор мотора - инструмент инфракрасного управления
 * 
 * v1.0, 29.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.03.2024
**/

#include "Motor_kru.h"  
#include "Admin_kru.h" 

// ****************************************************************************
// *                  Отработать команду ИКП (инфракрасного пульта)           *
// ****************************************************************************
void AdminKrutjak::Steward(MotorKrutjak Motor, uint16_t command) 
{
   // 1 - "стоп паровоз" - TV-VCR  
   if (command==1)
   { 
      Motor.Driver(514);
      delay(100);  
      //Serial.println("1 - стоп паровоз - TV-VCR");
   }
   // 20 - "начать движение" - REC     
   else if (command==20)
   {
      // Останавливаем
      Motor.Driver(514);
      delay(100);  
      // Вперёд
      if (DirectMove==true) 
      {
         Motor.Driver(543);
         delay(100);
         //Serial.println("20 - начать движение ВПЕРёД - REC");
      }
      // Назад
      else 
      {
         Motor.Driver(400); 
         delay(100);
         //Serial.println("20 - начать движение НАЗАД - REC");
      }
   }
   // 21 - "назад" - ==
   else if (command==21)
   {
      // Устанавливаем направление движения - назад
      DirectMove=false;  
      delay(100);
      //Serial.println("Устанавливаем направление движения - НАЗАД");
   }
   // 25 - "вперед" - =>
   else if (command==25)
   {
      // Устанавливаем направление движения - вперед
      DirectMove=true;  
      delay(100);
      //Serial.println("Устанавливаем направление движения - вперед");
   }
   else
   {
      Motor.Driver(550); 
      delay(100);
      //Serial.println(command);
   }
}
// ********************************************************** Admin_kru.cpp ***
