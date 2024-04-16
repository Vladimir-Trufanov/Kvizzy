/** KRUTJAK-SLAVE-PRO_MINI                           *** define_slave_kru.h ***
 * 
 * Определить переменные и константы исполнительной системы паровозика "КРУТЯК"
 * 
 * v3.0, 16.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 16.04.2024
**/

#ifndef define_slave_kru
#define define_slave_kru
#pragma once            

#define modeDebug  0              // отладочный режим на Arduino Uno
#define modeWork   1              // рабочий режим на Arduino Pro Mini
const int ModeSlave = modeDebug;  // текущий режим 

// Объявляем программный последовательный порт с RX=4, TX=3
SoftwareSerial serialSlave(4,3);  // желтый RX=4, зеленый TX=3

bool isTrass=true;                // true - трассировать принимаемые команды в последовательный порт

volatile char simb;               // последний принятый символ от управляющей системы
volatile uint16_t command = 0;    // последняя принятая команда управляющей системы
String strData = "";              // принятая последовательность символов
boolean recievedFlag = false;






/*

#define IR_RECEIVE_PIN   2        // контакт ИКП (инфракрасного приемника)
#define buzPin           3        // контакт активного зуммера
#define LEDPIN          13        // контакт контрольного светодиода
                                                           
iarduino_OLED_txt myOLED(0x3C);   // объявили объект OLED-дисплея по адресу на шине I2C: 0x3C 
extern uint8_t MediumFontRus[];   // подключили шрифт MediumFontRus.

bool      isFirst = false;        // признак первого запуска системы
bool      toggle1 = 0;
uint16_t  command = 0;            // код последней команды, принятой по ИКД

const int PinRes   = A0;  // аналоговый вход, к которому подключен потенциометр
const int PinPWM_L = 10;  // цифровой (ШИМ) выход, к которому подключено левое плечо транзисторов
const int PinPWM_R = 9;   // цифровой (ШИМ) выход, к которому подключено правое плечо

#define IR_RECEIVE_PIN 2


// Состояние светодиода: "горит" - "не горит"
bool doBurns;
// Определяем счетчик прерываний от таймера и общее их количество до
// события переключения светодиода = 1 секунде 
volatile unsigned int cntr;
const unsigned int BtnToggle = 62499;
// Определяем флаг истечения 1 сек для запуска трассировок
volatile boolean OneSecondFlag = false;
*/

#endif

// ***************************************************** define_slave_kru.h ***
