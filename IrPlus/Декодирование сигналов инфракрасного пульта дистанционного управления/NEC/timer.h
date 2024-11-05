//***************************************************************************
//  File........: timer.h
//
//  Author(s)...: Pashgan    chipenable.ru
//
//  Target(s)...: ATMega...
//
//  Compiler....: GNU GCC
//
//  Description.: Модуль для приемка ИК сигналов. Протокол фирмы NEC  
//
//  Data........: 18.04.11  
//
//***************************************************************************
#ifndef TIM_H
#define TIM_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd_lib.h"
#include "bcd.h"      
#include "bits_macros.h"

#define FCPU 16UL

void TIM_Init(void);       //инициализация таймера Т1
void TIM_Handle(void);     //обработка сигналов 
void TIM_Display(void);    //Вывод принятого кода на дисплей

#endif //TIM_H