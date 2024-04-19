/** KRUTJAK-MASTER-PRO_MINI                         *** define_master_kru.h ***
 * 
 * Определить переменные и константы управляющей системы паровозика "КРУТЯК"
 * 
 * v3.2, 19.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 16.04.2024
**/

#ifndef define_master_kru
#define define_master_kru
#pragma once            

#define IR_RECEIVE_PIN   2        // контакт ИКП (инфракрасного приемника)
#define buzPin           3        // контакт активного зуммера
#define sw5_RX           5        // RX=5
#define sw6_TX           6        // TX=6

#define LEDPIN          13        // контакт контрольного светодиода

// SDA-черный -синий    A4        // линия SDA программной шины I2C для OLED 
// SCL-красный-желтый   A5        // линия SCL программной шины I2C для OLED
                                                           
iarduino_OLED_txt myOLED(0x3C);   // объявили объект OLED-дисплея по адресу на шине I2C: 0x3D 
extern uint8_t MediumFontRus[];   // подключили шрифт MediumFontRus.

// Объявляем программный последовательный порт с RX(зеленый) =5, TX(желтый) =6
SoftwareSerial serialMaster(sw5_RX,sw6_TX); 

bool      isFirst = false;        // признак первого запуска системы
bool      doBurns=false;          // состояние контрольного светодиода

// Список контрольных величин вывода в монотор
uint16_t  command = 0;            // код последней команды, принятой по ИКД
String sCommand = "00";           // выводимое представление команды
String oldCommand = "99";         
float VccMaster=0.0;              // напряжение питания Master
float oldVccMst=0.0;              // прежнее напряжение питания Master
float VccSlave=0.0;               // напряжение питания Slave
float oldVccSlv=0.0;  
String sVcc;                      // представление напряжения в строке с 2 знаками росле зпт     

#endif

// **************************************************** define_master_kru.h ***
