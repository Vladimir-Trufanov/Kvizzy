
/** Arduino UNO *** trassirovka-signalov-ot-potenciometra-i-na-dvigatel.ino ***
 * 
 * Изучить взаимосвязанные сигналы от потенциометра и на двигатель у драйвера
 * электромотора
 * 
 * v1.0, 24.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 24.03.2024
**/

const int PinRes   = A0;     // аналоговый вход, к которому подключен потенциометр
const int PinPWM_L = 10;     // цифровой (ШИМ) выход, к которому подключено левое плечо транзисторов
const int PinPWM_R = 9;      // цифровой (ШИМ) выход, к которому подключено правое плечо

volatile int ValRes = 0;     // считанное значение напряжения с аналогового контакта (0-1023)
volatile int ValPWM_L = 0;   // хранимое значение ШИМ для отправки на затворы левого плеча (0-255)
volatile int ValPWM_R = 0;   // хранимое значение ШИМ для отправки на затворы правого плеча (0-255)

#define LEDPIN 13
// Определяем счетчик прерываний от таймера и общее их количество до
// события переключения светодиода  
volatile unsigned int cntr;
const unsigned int BtnToggle = 62499;

void setup() 
{
   MotorInit();
   TrassInit();   
}

void loop() 
{
   MotorDriver();
   TrassMake();   
}


// Драйвер мотора
void MotorInit() 
{
   pinMode(PinPWM_L, OUTPUT); // назначили, как выход, контакт 10 Arduino 
   pinMode(PinPWM_R, OUTPUT); 
}
void MotorDriver() 
{
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

// Трассировка управления мотором
void TrassInit() 
{
   pinMode(LEDPIN, OUTPUT);
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

void TrassMake() 
{
   if (cntr>BtnToggle)
   {
      //cli();
      digitalWrite(LEDPIN, !digitalRead(LEDPIN));
      cntr=0;
      //sei();
   }
}

ISR(TIMER2_OVF_vect)
{
   cntr=cntr+1;
}


// **************** trassirovka-signalov-ot-potenciometra-i-na-dvigatel.ino ***
