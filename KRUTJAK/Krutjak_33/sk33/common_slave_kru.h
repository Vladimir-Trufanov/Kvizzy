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
// *           Отработать команду по оборудованию исполняющей системы         *
// ****************************************************************************

/** Система команд ИК-пульта управления паровозиком
 * 
 *  1 - "стоп паровоз"        - TV-VCR
 *  2 -                       - ON standby
 *  3 - "начать движение"     - A.REP 
 * 20 - "начать движение"     - REC 
 * 21 - "назад"               - == 
 * 22 -                       - >|<    
 * 24 - "медленнее"           - <<  
 * 25 - "вперед"              - =>
 * 26 - "быстрее"             - >>   
 * 32 - "звук вкл/выкл"       - EJECT
 * 33 - "стоп паровоз"        - A.TRK
 * 44 - "подключить"          - TRACKING ON 
 * 45 - "отключить"           - TRACKING OFF 
 * 50 - "тестировать систему" - SYSTEM
 * 
**/

int actionCom(String command) 
{
   int reskom=0;   // "действие выполнено успешно"
   
   // "начать движение"
   if (command=="03") // A.REP
   {
      motor_speed(MAX_SPEED);
      motor_speed(START_SPEED);
   }
   if (command=="20") // REC
   {
      motor_speed(MAX_SPEED);
      motor_speed(START_SPEED);
   }
   // "стоп паровоз"
   if (command=="01") // TV-VCR
   {
      motor_speed(0);
   }
   if (command=="33") // A.TRK
   {
      motor_speed(0);
   }

   // "быстрее" 
   if (command=="26") // >>
   {
      // Реагируем в предположении, что мотор не работал
      if (currShim+DELTA_SPEED<MIN_SPEED) 
      {
         motor_speed(MAX_SPEED);
         motor_speed(MIN_SPEED);
      }
      else
      {
         if (currShim+DELTA_SPEED>MAX_SPEED) currShim=MAX_SPEED;
         else currShim=currShim+DELTA_SPEED;
      }
   }

   // "медленнее"   
   if (command=="24")
   {
      // Реагируем в предположении, что мотор не работал
      if (currShim<MIN_SPEED) 
      {
         motor_speed(MAX_SPEED);
         motor_speed(MIN_SPEED);
      }
      // Реагируем, когда мотор работает
      else
      {
         if (currShim-mm_SPEED<MIN_SPEED) currShim=MIN_SPEED;
         else currShim=currShim-mm_SPEED;
      }
   }
   // "назад" - если двигались вперед, то останавливаем движение 
   // и начинаем движение назад
   if (command=="21")
   {
      if (currDir==forward)
      {
         motor_speed(0);
         motor_direct(back);   
         motor_speed(MAX_SPEED);
         motor_speed(MIN_SPEED);
      }
   }
   // "вперед" - если двигались назад, то останавливаем движение 
   // и начинаем движение вперед
   if (command=="25")
   {
      if (currDir==back)
      {
         motor_speed(0);
         motor_direct(forward);   
         motor_speed(MAX_SPEED);
         motor_speed(MIN_SPEED);
      }
   }
   command=="00";
   return reskom;
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
