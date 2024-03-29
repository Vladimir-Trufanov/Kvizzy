/** Arduino UNO        *** trassirovka-signalov-ot-potenciometra-i-na-dvigatel.ino ***
 * 
 * Изучить взаимосвязанные сигналы от потенциометра и на двигатель у драйвера
 * электромотора
 * 
 * v1.3, 27.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 24.03.2024
**/

#include "Motor_kru.h"    // подключили драйвер мотора
#include "Irq_kru.h"      // подключили администратор мотора

const int PinRes   = A0;  // аналоговый вход, к которому подключен потенциометр
const int PinPWM_L = 10;  // цифровой (ШИМ) выход, к которому подключено левое плечо транзисторов
const int PinPWM_R = 9;   // цифровой (ШИМ) выход, к которому подключено правое плечо

#define LEDPIN 13            

// Определяем счетчик прерываний от таймера и общее их количество до
// события переключения светодиода = 1 секунде 
volatile unsigned int cntr;
const unsigned int BtnToggle = 62499;
// Инициируем начальное состояние светодиода - "не горит"
bool doBurns=false;

// Инициируем драйвер мотора
MotorKrutjak Motor(PinPWM_L,PinPWM_R,PinRes); 

// Инициируем структуры состояния мотора и
// текущего состояния таймерного прерывания
Condition Condition_Motor;
statusIrq CurrIrq;

void setup() 
{
   Serial.begin(9600);
   // Отсоединяем мотор
   Motor.Disconnect();
   TrassInit(cntr,LEDPIN);   
}

void loop() 
{
   Motor.Driver();
   CurrIrq=TrassMake(cntr,BtnToggle,doBurns,LEDPIN,Motor,Condition_Motor); 
   doBurns=CurrIrq.doBurns; 
   cntr=CurrIrq.cntr; 
}

SIGNAL(TIMER2_OVF_vect)
{
   cntr=cntr+1;
}

// ******************************* trassirovka-signalov-ot-potenciometra-i-na-dvigatel.ino ***
