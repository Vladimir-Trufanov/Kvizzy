/** KRUTJAK-SLAVE-PRO_MINI                           *** common_slave_kru.h ***
 * 
 * Определить общие функции управляющей системы паровозика "КРУТЯК"
 * 
 * v3.1, 21.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 19.04.2024
**/

#ifndef common_slave_kru
#define common_slave_kru
#pragma once            

// ****************************************************************************
// *          Подготовить и передать данные в управляющую систему             *
// ****************************************************************************
void sendState()
{
      /*
      if (ModeSlave==modeDebug) 
      {
         Serial.print("Vcc="); 
         Serial.println(VccSlave); 
      }
      */

      //serialSlave.println(sVcc+" "+sPwr+"#");
      // serialSlave.println(shim+" "+sPwr+"  345");

      //serialSlave.println("v="+sVcc+" p="+sPwr+" d="+d+" sh"+shim+".");
      //serialSlave.println("ATvcc="+sVcc+".");
      serialSlave.print(strInfo);
      delay(40); // выдержали паузу, чтобы команда спокойно ушла
}
// ****************************************************************************
// *                          Установить мощность мотора                      *
// ****************************************************************************
void motor_speed(byte iShim)
{
   currShim=iShim;
   analogWrite(PWM_PIN,currShim);
   delay(MS_TIME);
}
// ****************************************************************************
// *         Установить и отметить мощность мотора (для трассировки)          *
// *                             много раз через интервал                     *
// ****************************************************************************
/*
void motor_speedN(byte iShim, unsigned int nPutm = 0)
{
   if (OneMillSecFlag==true)
   {
      motor_speed(iShim)
      OneMillSecFlag = false;
   }
}
*/
// ****************************************************************************
// *    Установить и отметить направление вращения мотора (для трассировки)   *
// ****************************************************************************
void motor_direct(byte iDir)
{
   currDir=iDir;
   digitalWrite(DIR_PIN,currDir);
}

/*
// ****************************************************************************
// *               Установить тестовое трассировочное сообщение               *
// ****************************************************************************
String motor_infotest()
{
   String Result=sDir+sShim+": "+sPwr+"Вт   ";
   return Result;
}
*/

// ****************************************************************************
// *            Выполнить плавный разгон мотора (вперед или назад)            *
// ****************************************************************************
void motion_to_max(byte iDir)
{
   // Устанавливаем вращение мотора
   motor_direct(forward); 
   // Выключаем мотор
   motor_speed(0);
   delay(600);
   // Заводим мотор
   motor_speed(MAX_SPEED);
   // Медленно приращиваем скорость при помощи ШИМ
   for (uint8_t speed = UP_MIN_SPEED; speed < CTRL_SPEED; speed++) 
   {
      motor_speed(speed);
   }
   // Выходим на максимальную скорость
   motor_speed(CTRL_SPEED);
   delay(600);
   // Выключаем мотор
   motor_speed(0);
}



void move()
{
   delay(8000);
   analogWrite(PWM_PIN, MAX_SPEED);
   delay(MS_TIME);
   // Медленно приращиваем скорость при помощи ШИМ
   //for (uint8_t speed = UP_MIN_SPEED; speed < MAX_SPEED; speed++) 
   for (uint8_t speed = UP_MIN_SPEED; speed < 100; speed++) 
   {
      analogWrite(PWM_PIN, speed);
      delay(MS_TIME);
   }

   // Выходим на максимальную скорость
   //analogWrite(PWM_PIN, MAX_SPEED);
   analogWrite(PWM_PIN, 100);
   delay(2000);

   // Медленно сбавляем скорость
//   for (uint8_t speed = MAX_SPEED; speed > DOWN_MIN_SPEED; speed--) 
   for (uint8_t speed = 100; speed > DOWN_MIN_SPEED; speed--) 
   {
      analogWrite(PWM_PIN, speed);
      delay(MS_TIME);
   }

   // Выключаем мотор
   analogWrite(PWM_PIN, 0);
}

#endif

// ***************************************************** common_slave_kru.h ***
