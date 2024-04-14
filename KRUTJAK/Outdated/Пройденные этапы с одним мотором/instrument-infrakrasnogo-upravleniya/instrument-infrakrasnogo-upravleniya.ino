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

   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

   // Инициируем начальное состояние светодиода - "не горит"
   doBurns=false;
   // Настраиваем прерывание по переполнению 2 таймера 
   // для сигнализирования трассировок 
   //TrassInit();  
   // Отсоединяем мотор
   //Motor.Disconnect();
   //Motor.Driver(570);
   Motor.Driver(512);
   
   //Admin.Init(); 


  cli(); // отключили прерывания
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  sei(); // включили прерывания



   
}

void loop() 
{
   // !!! Здесь, в основном цикле программные задержки используются только при
   // отработке команды инфракрасного приемника = 0.2 сек



   


   // Проверяем флаг истечения 1 секунды, если флаг установлен (true), то
   // выполняем трассировку и передаём информацию внешнему контроллеру
   if (OneSecondFlag == true)
   {
    
      //Motor.Driver(600);
   
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

   
   if (IrReceiver.decode()) 
   {
      //Motor.Driver(512);
      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      uint16_t command = IrReceiver.decodedIRData.command;
      //Serial.print("Vcc0="); Serial.print(analogRead_VCC()); Serial.print("V\r\n");
      //Admin.Steward(Motor,command); 
      Serial.println(command);
      // Паузой отрезаем дребезг сигнала (повторный нулевой код)
      //delay(200);  
      //Serial.print("Vcc1="); Serial.print(analogRead_VCC()); Serial.print("V\r\n");
      // Готовим прием следующего нажатия клавиши
      IrReceiver.resume();
   }
   
   
   
}


ISR(TIMER0_COMPA_vect)
{
  
 
  //timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)

   // Увеличиваем счетчик прерываний 
   cntr=cntr+1;
   // Если счетчик дошел до 1 секунды, то выполняем подстройки
   if (cntr>BtnToggle)
   {
     //  Serial.println("com");
      // Снова инициализируем счетчик
      cntr=0;
      // Устанавливаем флаг прошествия 1 секунды.
      // Флаг всегда устанавливаем в true для того, чтобы основной цикл знал,
      // что секунда истекла (основной цикл по своей логике и сбросит флаг в false)
      OneSecondFlag = true;
   }

  /*
  
  if (toggle0){
    //digitalWrite(8,HIGH);
    digitalWrite(13,HIGH);
    toggle0 = 0;
  }
  else{
    //digitalWrite(8,LOW);
    digitalWrite(13,LOW);
    toggle0 = 1;
  }
  */
}


// ******************************* instrument-infrakrasnogo-upravleniya.ino ***
