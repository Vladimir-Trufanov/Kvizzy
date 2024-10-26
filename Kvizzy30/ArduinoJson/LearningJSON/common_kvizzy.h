/** Arduino, Esp32-CAM ************************************ common_kvizzy.h ***
 * 
 * Преобразовать json-документ в строку: https://arduinojson.org/     
 * 
 * v1.0, 26.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#ifndef common_kvizzy
#define common_kvizzy
#pragma once            

#include <Arduino.h>
#include <ArduinoJson.h>

// ****************************************************************************
// *                                      *
// ****************************************************************************
String jison1()
{
   String str = "***";
   // Allocate the JSON document
   JsonDocument doc;
   // Add values in the document
   doc["sensor"] = "gps";
   doc["time"] = 1351824120;
   // Add an array.
   JsonArray data = doc["data"].to<JsonArray>();
   data.add(48.756080);
   data.add(2.302038);

   serializeJson(doc,str);
   //serializeJsonPretty(doc,str);

   /*
   // Generate the minified JSON and send it to the Serial port.
   serializeJson(doc, Serial);
   // The above line prints:
   // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
   // Start a new line
   Serial.println();

   // Generate the prettified JSON and send it to the Serial port.
   //serializeJsonPretty(doc, Serial);
   // The above line prints:
   // {
   //   "sensor": "gps",
   //   "time": 1351824120,
   //   "data": [
   //     48.756080,
   //     2.302038
   //   ]
   // }
   */

   return str;
}

#endif

// ******************************************************** common_kvizzy.h ***
