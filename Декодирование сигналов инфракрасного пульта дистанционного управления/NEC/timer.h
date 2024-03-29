//***************************************************************************
//  File........: timer.h
//
//  Author(s)...: Pashgan    chipenable.ru
//
//  Target(s)...: ATMega...
//
//  Compiler....: GNU GCC
//
//  Description.: ������ ��� ������� �� ��������. �������� ����� NEC  
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

void TIM_Init(void);       //������������� ������� �1
void TIM_Handle(void);     //��������� �������� 
void TIM_Display(void);    //����� ��������� ���� �� �������

#endif //TIM_H