#include<Arduino.h>

bool throwError = true;
int dynamicValue;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  try 
  {
    delay(5000);
    if (throwError)
      dynamicValue = 0;
    else
      dynamicValue = 1;

    Serial.print("dynamicValue=");
    Serial.println(dynamicValue);
    // Есть исключение
    float error = 5 / dynamicValue;
    // Нет исключения
    // float error = 1 / dynamicValue;
    Serial.print("error=");
    Serial.println(error);

    //String uy="100 Привет!";
    //throw uy;
    int iuy=99;
    throw iuy;
  }

  catch (int cc) 
  {
    Serial.print("Исключение=");
    Serial.println(cc);
  }
  catch (String cc) 
  {
    Serial.print("Исключение=");
    Serial.println(cc);
  }
}
