/** Arduino, Esp32-CAM *************************************** JSON-Array.ino ***
 * 
 * Kvizzy30 - модельное программное обеспечение для Esp32-CAM и двух встроенных
 * светодиодов: контрольного и вспышки
 * 
 * v3.1, 07.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 31.05.2024
 * 
 *           Kvizzy - система контроллеров, датчиков и исполнительных устройств 
 *                    моего, стремящегося к умному, хозяйства (нижний уровень).
 * 
**/

/*
  JSON Value Extractor

  This sketch demonstrates how to use some features
  of the Official Arduino JSON library to traverse through all the
  key value pair in the object and the nested objects.
  Can be very helpful when searching for a specific data in a key
  which is nested at multiple levels
  The sketch actually use recursion to traverse all the keys in
  a given JSON.

  Example originally added on 24-03-2020
  by Madhur Dixit https://github.com/Chester-King

  This example code is in the public domain.
*/

/*
  JSON Array

  This sketch demonstrates how to use various features
  of the Official Arduino_JSON library, in particular for JSON arrays.

  This example code is in the public domain.
*/

#include <Arduino_JSON.h>

const char input[] = "[true, 42, \"apple\"]";

void setup() {
  Serial.begin(115200);
  while (!Serial);

  demoParse();

  demoCreation();
}

void loop() {
}

void demoParse() {
  Serial.println("parse");
  Serial.println("=====");

  JSONVar myArray = JSON.parse(input);

  // JSON.typeof(jsonVar) can be used to get the type of the variable
  if (JSON.typeof(myArray) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }

  Serial.print("JSON.typeof(myArray) = ");
  Serial.println(JSON.typeof(myArray)); // prints: array

  // myArray.length() can be used to get the length of the array
  Serial.print("myArray.length() = ");
  Serial.println(myArray.length());
  Serial.println();

  Serial.print("JSON.typeof(myArray[0]) = ");
  Serial.println(JSON.typeof(myArray[0]));

  Serial.print("myArray[0] = ");
  Serial.println(myArray[0]);
  Serial.println();

  Serial.print("myArray[1] = ");
  Serial.println((int) myArray[1]);
  Serial.println();

  Serial.print("myArray[2] = ");
  Serial.println((const char*) myArray[2]);
  Serial.println();

  Serial.println();
}

void demoCreation() {
  Serial.println("creation");
  Serial.println("========");

  JSONVar myArray;
  
  myArray[0] = false;
  myArray[1] = 4242.5;
  myArray[2] = "orange";
  myArray[3] = "world";
  myArray[4] = true;
  myArray[5] = 42;

  Serial.print("myArray.length() = ");
  Serial.println(myArray.length());

  // JSON.stringify(myVar) can be used to convert the JSONVar to a String
  String jsonString = JSON.stringify(myArray);

  Serial.print("JSON.stringify(myArray) = ");
  Serial.println(jsonString);
  Serial.println();

  for (int i = 0; i < myArray.length(); i++) {
    JSONVar value = myArray[i];

    Serial.print("JSON.typeof(myArray[");
    Serial.print(i);
    Serial.print("]) = ");
    Serial.println(JSON.typeof(value));

    Serial.print("myArray[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(value);

    Serial.println();
  }

  Serial.println();
}

// *********************************************************** JSON-Array.ino ***
