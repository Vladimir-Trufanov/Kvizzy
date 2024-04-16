/** KRUTJAK-MASTER-PRO_MINI                         *** define_master_kru.h ***
 * 
 * Определить переменные и константы управляющей системы паровозика "КРУТЯК"
 * 
 * v3.0, 16.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 16.04.2024
**/

#ifndef define_master_kru
#define define_master_kru
#pragma once            

#define IR_RECEIVE_PIN   2        // контакт ИКП (инфракрасного приемника)
#define buzPin           3        // контакт активного зуммера
#define LEDPIN          13        // контакт контрольного светодиода
                                                           
iarduino_OLED_txt myOLED(0x3C);   // объявили объект OLED-дисплея по адресу на шине I2C: 0x3C 
extern uint8_t MediumFontRus[];   // подключили шрифт MediumFontRus.

// Объявляем программный последовательный порт с RX=5, TX=6
SoftwareSerial serialMaster(5,6); // зеленый RX=5, желтый TX=6
// Определяем флаг истечения 1 сек для запуска трассировок
volatile boolean OneSecondFlag = false;

bool      isFirst = false;        // признак первого запуска системы
bool      toggle1 = 0;
bool      doBurns=false;          // состояние контрольного светодиода

// Список контрольных величин вывода в монотор
uint16_t  command = 0;            // код последней команды, принятой по ИКД
String sCommand = "00";           // выводимое представление команды
String oldCommand = "99";         

/*
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

// **************************************************** define_master_kru.h ***
