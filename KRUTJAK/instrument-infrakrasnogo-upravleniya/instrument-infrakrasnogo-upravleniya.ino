/** Arduino UNO                *** instrument-infrakrasnogo-upravleniya.ino ***
 * 
 * Построить модель администратора мотора - инструмента инфракрасного управления
 * 
 * v1.0, 29.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.03.2024
**/

#include <IRremote.h>
#include <iarduino_VCC.h> 

#include "Motor_kru.h"    // подключили драйвер мотора
#include "Admin_kru.h"    // подключили драйвер мотора
#include "Irq_kru.h"      // подключили администратор мотора

const int PinRes   = A0;  // аналоговый вход, к которому подключен потенциометр
const int PinPWM_L = 10;  // цифровой (ШИМ) выход, к которому подключено левое плечо транзисторов
const int PinPWM_R = 9;   // цифровой (ШИМ) выход, к которому подключено правое плечо

#define LEDPIN         13 
#define IR_RECEIVE_PIN 2

// Определяем счетчик прерываний от таймера и общее их количество до
// события переключения светодиода = 1 секунде 
volatile unsigned int cntr;
const unsigned int BtnToggle = 62499;
// Определяем флаг истечения 1 сек для запуска трассировок
volatile boolean OneSecondFlag = false;

// Инициируем начальное состояние светодиода - "не горит"
bool doBurns=false;

boolean toggle1 = 0;

// Инициируем драйвер мотора
MotorKrutjak Motor(PinPWM_L,PinPWM_R,PinRes); 
// Инициируем aдминистратор мотора
AdminKrutjak Admin; 

// Инициируем структуры состояния мотора и
// текущего состояния таймерного прерывания
Condition Condition_Motor;
statusIrq CurrIrq;

void setup() 
{
   Serial.begin(9600);
   //IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

   // Настраиваем прерывание по переполнению 2 таймера 
   // для сигнализирования трассировок 
   TrassInit(cntr,LEDPIN);  
   // Отсоединяем мотор
   Motor.Disconnect();

   /*
   pinMode(13, OUTPUT);
   */
   
   //Admin.Init(); 

   /*
   Motor.Driver(900);
   Serial.println(900);
   delay(1000);
   Motor.Driver(1000);
   Serial.println(1000);
   delay(1000);
   Motor.Driver(1024);
   Serial.println(1024);
   delay(1000);
   */
}

void loop() 
{

   delay(1000);
   Motor.Driver(600);
   
   // Проверяем флаг истечения 1 секунды, если флаг установлен (true), то
   // выполняем трассировку и передаём информацию внешнему контроллеру
   if (OneSecondFlag == true)
   {
      Serial.print("Vcc="); 
      Serial.print(analogRead_VCC()); // вывели напряжение питания
      Serial.print("V\r\n");
      OneSecondFlag=false;            // сбросили флаг одной секунды
   }
  
   /*
   Motor.Driver(512);
   delay(1000);  
   //Motor.Driver(545);
   Motor.Driver(465);
   delay(1000);
   */
 
   /*
   delay(5000);  
   Motor.Driver(512);
   Serial.print( "Vcc = "         );                 // 
   Serial.print( analogRead_VCC() );                 // Выводим напряжение питания.
   Serial.print( " V.\r\n"        );                 // 
   Serial.println(512);
   delay(2000);  
   Motor.Driver(600);
   Serial.println(600);
   Serial.print( "Vcc = "         );                 // 
   Serial.print( analogRead_VCC() );                 // Выводим напряжение питания.
   Serial.print( " V.\r\n"        );                 // 
   delay(2000);
   */
   
   /*
   Motor.Driver(700);
    Serial.print( "Vcc = "         );                 // 
    Serial.print( analogRead_VCC() );                 // Выводим напряжение питания.
    Serial.print( " V.\r\n"        );                 // 
   Serial.println(700);
   delay(2000);
   */

   /*   
   Motor.Driver(800);
    Serial.print( "Vcc = "         );                 // 
    Serial.print( analogRead_VCC() );                 // Выводим напряжение питания.
    Serial.print( " V.\r\n"        );                 // 
   Serial.println(800);
   delay(2000);
   
   Motor.Driver(1025);
    Serial.print( "Vcc = "         );                 // 
    Serial.print( analogRead_VCC() );                 // Выводим напряжение питания.
    Serial.print( " V.\r\n"        );                 // 
   Serial.println(1025);
   delay(2000);
   
   Motor.Driver(512);
    Serial.print( "Vcc = "         );                 // 
    Serial.print( analogRead_VCC() );                 // Выводим напряжение питания.
    Serial.print( " V.\r\n"        );                 // 
   Serial.println(512);
   delay(2000);
   */
   
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

   // Отрабатываем блок трассировки
   //CurrIrq=TrassMake(cntr,BtnToggle,doBurns,LEDPIN,Motor,Condition_Motor); 
   //doBurns=CurrIrq.doBurns; 
   //cntr=CurrIrq.cntr; 
}
// ****************************************************************************
// *               Обработать прерывание по переполнению 2 таймера            *
// ****************************************************************************
SIGNAL(TIMER2_OVF_vect)
{
   // Увеличиваем счетчик прерываний 
   cntr=cntr+1;
   // Если счетчик дошел до 1 секунды, то выполняем подстройки
   if (cntr>BtnToggle)
   {
      // Снова инициализируем счетчик
      cntr=0;
      // Устанавливаем флаг прошествия 1 секунды.
      // Флаг всегда устанавливаем в true для того, чтобы основной цикл знал,
      // что секунда истекла (основной цикл по своей логике и сбросит флаг в false)
      OneSecondFlag = true;
   }
}

// ******************************* instrument-infrakrasnogo-upravleniya.ino ***
