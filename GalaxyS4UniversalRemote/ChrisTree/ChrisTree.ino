
#include <IRremote.hpp>
//#include "IRremote.h"

IRrecv irrecv(2);       // указываем вывод, к которому подключен приемник
decode_results results;

// Делаем код кнопки "Включить ёлку"
String cx="16722135";
// Делаем код кнопки "ВЫКЛ ёлку"
String cy="16713975";

void setup() 
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(3, OUTPUT);
  Serial.begin(9600);  // выставляем скорость COM порта
  irrecv.enableIRIn(); // запускаем прием
}

void loop() 
{
  // если данные пришли
  if (irrecv.decode(&results)) 
  { 
    String cc=(String)results.value;
    Serial.println(cx); 
    Serial.println(cy); 
    Serial.println(cc); 
    Serial.println(results.value);      // печатаем данные
    
    if (cx==cc) 
    {
      Serial.println("ВКЛЮЧАЕМ!");
      //digitalWrite(13, HIGH);
      digitalWrite(3, HIGH);
    }
    
    if (cy==cc) 
    {
      Serial.println("ВыкЛ!");
      //digitalWrite(13, LOW);
      digitalWrite(3, LOW);
    }
    Serial.println(" ");      // печатаем данные
    
    /*
    int zx=16712445;
    if (results.value==zx) Serial.println("Получилось!"); 
    if ((String)results.value==(String)zx) Serial.println("Получилось!"); 
    */
    
    irrecv.resume(); // принимаем следующую команду
  }
}
