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

#define sw3_TX     3        // последовательный порт: TX=3
#define sw4_RX     4        // последовательный порт: RX=4

#define DIR_PIN    8        // направление движения мотора 
#define PWM_PIN    9        // ШИМ-сигнал на мотор (0-255)

// Объявляем программный последовательный порт с RX(желтый)=4, TX(зеленый)=3
SoftwareSerial serialSlave(sw4_RX,sw3_TX); 

volatile char simb;               // последний принятый символ от управляющей системы
volatile uint16_t command = 0;    // последняя принятая команда управляющей системы
String strData = "";              // принятая последовательность символов
bool recievedFlag = false;        // флаг завершения сборки командной последовательности
int reskom=0;                     // состояние ИСКР после выполнения действия по команде

// Определяем контрольные значения ШИМ
#define MAX_SPEED      255        // максимум
#define UP_MIN_SPEED    64        // минимум на повышении скорости
#define DOWN_MIN_SPEED  64        // минимум на понижении скорости
#define MS_TIME         20        // !
#define forward          0        // направление движения = "вперёд" 
#define back             1        // направление движения = "назад" 

// Данные о напряжении батареи и мощности двигателя
float  VccSlave=0.0;              // напряжение питания Slave
String sVcc;                      // представление напряжения в строке с 2 знаками после зпт     
byte   currShim;                  // текущий ШИМ на мотор
byte   currDir;                   // текущее направление движения мотора
float  pwrSlave=0.0;              // мощность ШИМ на моторе
String sPwr;                      // представление мощности в строке с 2 знаками после зпт     

#endif

// ***************************************************** define_slave_kru.h ***