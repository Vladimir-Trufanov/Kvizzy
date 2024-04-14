/** Arduino UNO *************** *** ikd-tranzistor-motor.ino ***
 * 
 * 
 * v1.1, 12.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 10.03.2024
**/

//#include <IRremote.h>
#include <SoftwareSerial.h>
SoftwareSerial serialRX(4,3);   // RX=4, TX=3

#define IR_RECEIVE_PIN 11

const int PinRes = A0;  // аналоговый вход, к которому подключен потенциометр
const int PinPWM = 10;  // цифровой (ШИМ) выход, к которому подключен затвор полевого транзистора
      int ValRes = 0;   // считанное значение напряжения с аналогового контакта (0-1023)
volatile int ValPWM;    // расчитанное значение ШИМ для отправки на затвор (0-255)

// Состояние светодиода: "горит" - "не горит"
bool doBurns;
// Определяем счетчик прерываний от таймера и общее их количество до
// события переключения светодиода ~1 секунде 
volatile unsigned int cntr;
const unsigned int BtnToggle = 4000; //62499;
// Определяем флаг истечения 1 сек для запуска трассировок
volatile boolean OneSecondFlag = false;

volatile char simb;
volatile int isimb;

void setup() 
{
   pinMode(PinPWM, OUTPUT); // назначили, как выход, контакт 10 Arduino 
   pinMode(IR_RECEIVE_PIN, INPUT_PULLUP); 

   serialRX.begin(300);  

   

   Serial.begin(9600);
   pinMode(13,OUTPUT);
   
   // IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
    
  cntr=0;
  doBurns=false;
  ValPWM = 0;



   // 
   // Установить 2 таймер на частоту прерывания 2kHz, чтобы можно было 
   // пользоваться delay(), millis()
   //
   // Timer0 -  8 бит, счетный регистр   [0,255], используется delay(), millis()
   // Timer1 - 16 бит, счетный регистр [0,65535], используется Arduino Servo
   // Timer2 -  8 бит, счетный регистр   [0,255], используется tone()
 
   cli();                       // отключили прерывания
   TCCR2A = 0;
   TCCR2B = 0;
   TCNT2  = 0;
   OCR2A = 124;                 // =(16*10^6)/(2000*64)-1 (менее 256)
   TCCR2A|=(1<<WGM01);          // включили режим сравнения (CTC)
   TCCR2B|=(1<<CS01)|(1<<CS00); // установили биты CS01,CS00 на прескалер 64 
   TIMSK2 |= (1<<OCIE2A);       // включили таймер на прерывание по сравнению
   sei();                       // включили прерывания

   ValPWM = 230;
   analogWrite(PinPWM,ValPWM);        
  //    delay(2000);

}

void loop() 
{
   analogWrite(PinPWM,ValPWM);

   int But = digitalRead(IR_RECEIVE_PIN);
   
   if (serialRX.available())
  {
    //while(serialRX.available())
    //{
       simb=serialRX.read();
       String str(simb);
       Serial.print(simb);
       Serial.print(str);
       if (str=="2") analogWrite(PinPWM,200);  
    //}
  }
   
    
      
      
      
      //ValPWM = 255;
      //analogWrite(PinPWM,ValPWM);        
      //delay(200);

   //ValRes = analogRead(PinRes);           // считали напряжение с потенциометра
   //ValPWM = map(ValRes, 0,1023, 0,255);   // расчитали напряжение ШИМ для отправки на затвор транзистора
   //analogWrite(PinPWM,ValPWM);            // установили скорость мотора в дискретах
   
   // Проверяем флаг истечения 1 секунды, если флаг установлен (true), то
   // выполняем трассировку и передаём информацию внешнему контроллеру
   if (OneSecondFlag == true)
   {
      if (But==0)
      {
         ValPWM = 0;
      }
      //else  analogWrite(PinPWM,0);        

      OneSecondFlag = false; 
      doBurns=!doBurns;
      digitalWrite(13,doBurns);
      Serial.println(ValPWM);
      Serial.println(But);
   }


   

   /*   
   if (IrReceiver.decode()) 
   {
      //ValPWM = 255;
      //analogWrite(PinPWM,ValPWM);        
      //delay(2000);
      //ValPWM = 0;
      //analogWrite(PinPWM,ValPWM);        
      //delay(200);
    
      //Motor.Driver(512);
      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      uint16_t command = IrReceiver.decodedIRData.command;
      //Serial.print("Vcc0="); Serial.print(analogRead_VCC()); Serial.print("V\r\n");
      //Admin.Steward(Motor,command); 
      //Serial.println(command);
      // Паузой отрезаем дребезг сигнала (повторный нулевой код)
      //delay(200);  
      //Serial.print("Vcc1="); Serial.print(analogRead_VCC()); Serial.print("V\r\n");
      // Готовим прием следующего нажатия клавиши
      IrReceiver.resume();
   }
   */
   
}


ISR(TIMER2_COMPA_vect)
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

// ******************************** upravlyaem-skorostyu-ehlektromotora.ino ***
