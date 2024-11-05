#include "timer.h"

#define PRE 64UL

#define START_IMP_TH  (12000UL*FCPU)/PRE
#define START_IMP_MAX (15000UL*FCPU)/PRE
#define BIT_IMP_MAX   (3000UL*FCPU)/PRE
#define BIT_IMP_TH    (1500UL*FCPU)/PRE

volatile unsigned int icr1 = 0;
volatile unsigned int icr2 = 0;
enum state {IDLE = 0, RESEIVE = 1};  
enum state currentState = IDLE;

#define CAPTURE    0
#define RESEIVE_OK 1
volatile unsigned char flag = 0;

//первые четыре байта - адрес и команда, пятый байт - количество повторов
#define NUM_REPEAT 4
#define MAX_SIZE 5
unsigned char buf[MAX_SIZE];

//инициализация таймера Т1
void TIM_Init(void)
{
   DDRD &= ~(1<<PD6);
   PORTD |= (1<<PD6);
  
   TIMSK = (1<<TICIE1); //разрешаем прерывание по событию захват
   TCCR1A=(0<<COM1A1)|(0<<COM1A0)|(0<<WGM11)|(0<<WGM10);  //режим - normal, 
   TCCR1B=(0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(0<<WGM12)|(0<<CS12)|(1<<CS11)|(1<<CS10); //захват по заднему фронту, предделитель - 64
   TCNT1 = 0;
   
   currentState = IDLE;
}


//прерывание по событию захват
ISR(TIMER1_CAPT_vect)
{

   icr1 = icr2;
   icr2 = ICR1;  
   SetBit(flag, CAPTURE);
}

unsigned int TIM_CalcPeriod(void)
{
  unsigned int buf1, buf2;
  
  cli();
  buf1 = icr1;
  buf2 = icr2;
  sei(); 
  
  if (buf2 > buf1) {
    buf2 -= buf1;
  }
  else {
    buf2 += (65535 - buf1);    
  }
  return buf2;
}

//
void TIM_Handle(void)
{
  unsigned int period;
  static unsigned char data; 
  static unsigned char countBit, countByte;

  if (BitIsClear(flag, CAPTURE)) return;
 
  period = TIM_CalcPeriod();
  
  switch(currentState){
      //ждем стартовый импульс
      case IDLE:
         if (period < START_IMP_MAX) {
           if (period > START_IMP_TH){
             data = 0;
             countBit = 0;
             countByte = 0;
             buf[NUM_REPEAT] = 0;
             currentState = RESEIVE;
           }
           else {
             buf[NUM_REPEAT]++;
           }
         }
         break;
       
       //прием посылки
       case RESEIVE:
         if (period < BIT_IMP_MAX){
           if (period > BIT_IMP_TH){
              SetBit(data, 7);
           }
           countBit++;
           if (countBit == 8){
             buf[countByte] = data;
             countBit = 0;
             data = 0;
             countByte++;
             if (countByte == (MAX_SIZE - 1)){
               SetBit(flag, RESEIVE_OK);
               currentState = IDLE;
               break;
             }             
           }
           data = data>>1;
         }
         break;
         
               
       default:
          break;
    }
 
  ClearBit(flag, CAPTURE);
}


void TIM_Display(void)
{
  if(BitIsSet(flag, RESEIVE_OK)){
    LCD_Goto(0,0);
    BCD_3Lcd(buf[0]);
    LCD_WriteData(' ');
    BCD_3Lcd(buf[1]);
    LCD_WriteData(' ');
    BCD_3Lcd(buf[2]);
    LCD_WriteData(' ');
    BCD_3Lcd(buf[3]);
    LCD_WriteData(' ');
    ClearBit(flag, RESEIVE_OK);
  }

  LCD_Goto(0,1);  
  BCD_3Lcd(buf[NUM_REPEAT]);
}