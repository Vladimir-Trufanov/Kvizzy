/** Arduino, Esp32-CAM ************************************ define_kvizzy.h ***
 * 
 * Определить переменные и константы нижнего уровня умного хозяйства на двух светодиодах
 * 
 * v1.1, 26.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            

#include <Arduino.h>
// Строим json-документ
String thisController() 
{
   doc["namectrl"] = "Esp32-CAM во двор дачи";
   doc["idplace"]  = 201;   // 'Во двор дачи'
   doc["tidctrl"]  = 1;     // 'Esp32-CAM'
   doc["idctrl"]   = 201;   // идентификатор контроллера
   // Add values in the document
   doc["sensor"] = "gps";
   doc["time"] = 1351824120;
   // Add an array.
   JsonArray data = doc["data"].to<JsonArray>();
   data.add(48.756080);
   data.add(2.302038);
   String str = "";
   serializeJsonPretty(doc,str);
   //serializeJson(doc,str);
   return str;
}
// Строим выжимку json-документа
String thisMake(String sjson) 
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
   serializeJsonPretty(doc,str);
   //serializeJson(doc,str);
   return str;
}
// Делаем замену в глобальном json-документе
String Proba(String sjson) 
{
   deserializeJson(doc, sjson);
   doc["data"][0]=18.7;
   String str = "";
   serializeJsonPretty(doc,str);
   //serializeJson(doc,str);
   return str;
}

// ******************************************************** define_kvizzy.h ***
