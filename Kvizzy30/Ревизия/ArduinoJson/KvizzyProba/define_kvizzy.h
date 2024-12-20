/** Arduino, Esp32-CAM ************************************ define_kvizzy.h ***
 * 
 * Определить переменные и константы нижнего уровня умного хозяйства на двух светодиодах
 * 
 * v1.1, 26.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#ifndef define_kvizzy
#define define_kvizzy
#pragma once            

#include <Arduino.h>

String thisController() 
{
   doc["namectrl"] = "Esp32-CAM во двор дачи";
   doc["idplace"]  = 201;   // 'Во двор дачи'
   doc["tidctrl"]  = 1;     // 'Esp32-CAM'
   doc["idctrl"]   = 201;   // идентификатор контроллера
   String str = "";
   serializeJson(doc,str);
   return str;
}

String thisA(String sjson) 
{
   JsonDocument filter;
   filter["namectrl"] = true; // "Esp32-CAM во двор дачи";
   filter["idplace"]  = true; // 'Во двор дачи'
   filter["tidctrl"]  = true; // 'Esp32-CAM'
   filter["idctrl"]   = true; // идентификатор контроллера
   // Deserialize the document
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   String str = "";
   serializeJson(doc,str);
   return str;
}

String thisB(String sjson) 
{
   JsonDocument filter;
   filter["sensor"] = true; 
   filter["time"]  = true; 
   filter["data"]  = true; 
   JsonDocument doc;
   deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   doc["data"][0]=1.0;
   doc["data"][1]=1.1;
   String str = "";
   serializeJson(doc,str);
   return str;
}

/*
const int PinRes   = A0;  // аналоговый вход, к которому подключен потенциометр
const int PinPWM_L = 10;  // цифровой (ШИМ) выход, к которому подключено левое плечо транзисторов
const int PinPWM_R = 9;   // цифровой (ШИМ) выход, к которому подключено правое плечо

#define LEDPIN         13 
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

// ******************************************************** define_kvizzy.h ***
