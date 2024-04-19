/** KRUTJAK-SLAVE-PRO_MINI                           *** define_slave_kru.h ***
 * 
 * Определить переменные и константы исполнительной системы паровозика "КРУТЯК"
 * 
 * v3.1, 19.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 16.04.2024
**/

// ИСКР - исполнительная система паровозика "КРУТЯК"

#ifndef define_slave_kru
#define define_slave_kru
#pragma once            

#define modeDebug  0              // отладочный режим на Arduino Uno
#define modeWork   1              // рабочий режим на Arduino Pro Mini
const int ModeSlave = modeDebug;  // текущий режим 

#define sw3_TX           3        // TX=6
#define sw4_RX           4        // RX=4

// Объявляем программный последовательный порт с RX(желтый)=4, TX(зеленый)=3
SoftwareSerial serialSlave(sw4_RX,sw3_TX); 

volatile char simb;               // последний принятый символ от управляющей системы
volatile uint16_t command = 0;    // последняя принятая команда управляющей системы
String strData = "";              // принятая последовательность символов
bool recievedFlag = false;        // флаг завершения сборки командной последовательности
int reskom=0;                     // состояние ИСКР после выполнения действия по команде
float VccSlave=0.0;               // напряжение питания Slave
String sVcc;                      // представление напряжения в строке с 2 знаками росле зпт     

#endif

// ***************************************************** define_slave_kru.h ***
