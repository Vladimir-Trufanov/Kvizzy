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
// *                    Установить направление вращения мотора                *
// ****************************************************************************
void motor_direct(byte iDir)
{
   currDir=iDir;
   digitalWrite(DIR_PIN,currDir);
}

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
   for (uint8_t speed = MIN_SPEED; speed < START_SPEED; speed++) 
   {
      motor_speed(speed);
   }
   // Выходим на максимальную тестовую скорость
   motor_speed(START_SPEED);
   delay(600);
   // Выключаем мотор
   motor_speed(0);
}

#endif

// ***************************************************** common_slave_kru.h ***
