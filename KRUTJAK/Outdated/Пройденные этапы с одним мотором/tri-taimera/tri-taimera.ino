
/** Arduino UNO ******************************************* *** tri-taimera.ino ***
 *  (based on timer interrupts by Amanda Ghassaei June 2012)
 * 
 * Обеспечить мигание светодиода с интервалом в 1 сек (1Гц) на плате через 
 * прерывания 16-тиразрядного первого таймера - Timer1 по событию совпадения 
 * с регистром сравнения (то есть в режиме CTC)
 * 
 * v1.2, 21.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 20.03.2024
**/



//timer interrupts
//by Amanda Ghassaei
//June 2012
//https://www.instructables.com/id/Arduino-Timer-Interrupts/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/

//timer setup for timer0, timer1, and timer2.
//For arduino uno or any board with ATMEL 328/168.. diecimila, duemilanove, lilypad, nano, mini...

//this code will enable all three arduino timer interrupts.
//timer0 will interrupt at 2kHz
//timer1 will interrupt at 1Hz
//timer2 will interrupt at 8kHz

//storage variables
boolean toggle0 = 0;
boolean toggle1 = 0;
boolean toggle2 = 0;

void setup()
{
  
  //set pins as outputs
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);

cli();//stop interrupts

  
  //set timer0 interrupt at 2kHz
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

  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);


sei();//allow interrupts

}//end setup

ISR(TIMER0_COMPA_vect)
{//timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle0){
    //digitalWrite(8,HIGH);
    //digitalWrite(13,HIGH);
    toggle0 = 0;
  }
  else{
    //digitalWrite(8,LOW);
    //digitalWrite(13,LOW);
    toggle0 = 1;
  }
}

ISR(TIMER1_COMPA_vect)
{//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle1){
    digitalWrite(13,HIGH);
    toggle1 = 0;
  }
  else{
    digitalWrite(13,LOW);
    toggle1 = 1;
  }
}
  
ISR(TIMER2_COMPA_vect)
{//timer1 interrupt 8kHz toggles pin 9
//generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle2){
    //digitalWrite(9,HIGH);
    //digitalWrite(13,HIGH);
    toggle2 = 0;
  }
  else{
    //digitalWrite(9,LOW);
    //digitalWrite(13,LOW);
    toggle2 = 1;
  }
}


void loop(){
  //do other things here
}








/*
// Инициируем переключатель светодиода - false
boolean toggle1 = 0;

void setup()
{
  pinMode(13, OUTPUT);
  cli(); // остановили прерывания
  
  // Очищаем биты регистров таймера
  TCCR1A = 0; // сбросили биты первого конфигурационного регистра
  TCCR1B = 0; // сбросили биты второго конфигурационного регистра
  TCNT1  = 0; // сбросили счетный регистр
  
  // Замечание: для использования режима СТС с целью получения интервала в одну 
  //    секунду нужно определить два значения для размещения их в 
  // регистрах таймера -
  //    предделитель подбираем равным 1024, что обеспечивает изменение 
  // счетчика (счетного регистра) не по каждому такту контроллера, а по 
  // прошествии 1024 тактов;
  //    значение счетчика определяем следующим образом за несколько шагов:
  
  // 1. Разрешение таймера = 1 / (тактовая частота контроллера / предделитель)
  
  // 2. Значение счетчика = интервал таймера / разрешение таймера - 1
  // (дополнительная единица добавляется к счетчику потому, что при совпадении 
  // счетчика с заданным значением он сбросится в ноль. Сброс занимает один 
  // тактовый период, который надо учесть в расчетах. Во многих случаях ошибка 
  // в один период не слишком значима, но в высокоточных задачах она может быть 
  // критичной.)

  // Разрешение таймера = 1 / (16000000 / 1024) = 0,000064 (cек)
  // Значение счетчика  = 1 / 0,000064 - 1      = 15624
   
  // Устанавливаем значение регистра сравнения для частоты на прерывание 1Гц
  OCR1A = 15624; // 1/(16000000/1024)-1 (должно быть <= 65535 = 2^16 степени-1)
  // Устанавливаем CTC режим для прерывания
  TCCR1B |= (1 << WGM12);
  // Устанавливаем биты для получения значения предделителя (prescaler) равного 1024 
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // Разрешаем прерывание первого таймера (канал А) по сравнению - СТС
  TIMSK1 |= (1 << OCIE1A);
  sei(); // разрешили прерывания
}
*/

/** 
 * Обработать прерывание по сравнению от первого счетчика на канале А
 * с частотой 1 Гц
**/
/*
ISR(TIMER1_COMPA_vect)
{
  // Если переключатель включен, то переводим пин светодиода в
  // верхний уровень и сбрасываем переключатель
  if (toggle1)
  {
    digitalWrite(13,HIGH);
    toggle1 = 0;
  }
  // Если переключатель ВЫКЛ, то переводим пин светодиода в
  // НИЖНИЙ уровень и устанавливаем переключатель
  else
  {
    digitalWrite(13,LOW);
    toggle1 = 1;
  }
}

void loop()
{
}
*/
// ************************************************************ tri-taimera.ino ***
