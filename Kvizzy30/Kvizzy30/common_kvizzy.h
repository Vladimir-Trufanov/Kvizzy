/** Arduino, Esp32-CAM ************************************ common_kvizzy.h ***
 * 
 * Определить общие функции нижнего уровня умного хозяйства на двух светодиодах
 * 
 * v1.0, 26.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>

// ****************************************************************************
// *                      --Преобразовать json-документ в строку              *
// *                      --https://arduinojson.org/                          *
// ****************************************************************************
String jison1()
{
   String str = "***";
   return str;
}


// Может пригодится!!!

// Сделать определитель типов:
#define tstr "tstr"
#define tchr "tchr"
#define tint "tint"
String types(String a) {return tstr;}
String types(char *a)  {return tchr;}
String types(int a)    {return tint;}

// ******************************************************** common_kvizzy.h ***
