/** KRUTJAK-MASTER-PRO_MINI ****************************** *** mk04-Serial.ino ***
 * 
 * 1 - По клавише "System" запустить тестирование системы (пока три пика зумера)
 * 
 * v1.1, 14.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 12.04.2024
**/

#include <IRremote.hpp>
#include <iarduino_VCC.h> 

#define IR_RECEIVE_PIN 2
#define buzPin 3
                                                             // 
#include <iarduino_OLED_txt.h>                               // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x3C);                              // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x3C или 0x3D.

#include <SoftwareSerial.h>
SoftwareSerial serialTX(5,6);  // RX=5, TX=6
                                                             
extern uint8_t MediumFontRus[];                              // Подключаем шрифт MediumFontRus.

bool isFirst = false;
bool toggle1 = 0;
uint16_t command = 0;

void setup() 
{

   myOLED.begin();                                          // Инициируем работу с дисплеем.
   myOLED.setFont(MediumFontRus);                           // Указываем шрифт который требуется использовать для вывода цифр и текста.

   serialTX.begin(300);  

   pinMode(buzPin, OUTPUT);  
   digitalWrite(buzPin, LOW);
   IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);

    pinMode(13, OUTPUT);
 
cli();//stop interrupts
 
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

sei();//allow interrupts

}

void loop() 
{
   if (isFirst==false)
   {
      // Подключаем блок питания SLAVE
      // Делаем задержку и трассируем включение
      delay(1000);  
      buzz_Ok();
      myOLED.clrScr(); // Чистим экран.
      isFirst=true;
   }
   
   if (IrReceiver.decode()) 
   {
      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      command = IrReceiver.decodedIRData.command;
      if (command==50)
      {
         buzz_Ok();
      }
      else
      {
         // Отправляем команду в Slave
         // Отправляем AT-команды на VirtUART
         serialTX.println("AT+2");    // "поставить на паузу"
         myOLED.print(command,80,1);                  // Выводим текст начиная с 92 столбца 1 строки (высота шрифта 2 строки, он займёт строки 0 и 1).
      }
      // Делаем задержку в 100 мс, чтобы отработать сигнал от нажатия клавиши 
      delay(100);  
      // Готовим прием следующего нажатия клавиши
      IrReceiver.resume();
   }
}

void buzz_Ok()
{
         digitalWrite(buzPin, HIGH);
         delay(100);  
         digitalWrite(buzPin, LOW);
         delay(100);  
         digitalWrite(buzPin, HIGH);
         delay(100);  
         digitalWrite(buzPin, LOW);
         delay(100);  
         digitalWrite(buzPin, HIGH);
         delay(100);  
         digitalWrite(buzPin, LOW);
}

ISR(TIMER1_COMPA_vect)
{
   // timer1 interrupt 1Hz toggles pin 13 (LED)
   // generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
   if (toggle1)
   {
      digitalWrite(13,HIGH);
      toggle1 = 0;
   }
   else
   {
      digitalWrite(13,LOW);
      toggle1 = 1;
   }

   // Выводим последнюю команду
   // myOLED.print(F("ком"),   0,     1);  
   // myOLED.print(command,  40,     1);  
   myOLED.print(F("iArduino"), 15,     4);                  // Выводим текст начиная с 15 столбца 4 строки (высота шрифта 2 строки, он займёт строки 3 и 4).
   // Выводим напряжения батарей
   myOLED.print(F("v"),0,7); myOLED.print(F(":"),10,7);                 
   myOLED.print(F("4.99"),22,7); myOLED.print(F("-"),69,7); myOLED.print(analogRead_VCC(),80,7);                   
}


// ********************************************************* mk04-Serial.ino ***
